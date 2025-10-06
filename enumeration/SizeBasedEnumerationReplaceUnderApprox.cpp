
#include "SizeBasedEnumerationReplaceUnderApprox.h"

#include <future>
#include <boost/log/trivial.hpp>

#include "../LevenshteinAutomatonCreator.h"

void createAutomaton(LevenshteinAutomatonCreator creator, std::shared_ptr<std::promise<mata::nfa::Nfa>> prom, std::shared_ptr<std::atomic_bool> finished) {
    auto myret = creator.createLevenshteinAutomaton();
    prom->set_value(std::move(myret));
    finished->store(true);
    // TODO this is unsafe
}

mata::nfa::Nfa SizeBasedEnumerationReplaceUnderApprox::createNfaForContext(int contextIndex) {
    LevenshteinAutomatonCreator creator(alphabetPerContext.at(contextIndex), radius, expectedOutputsVals.at(contextIndex));
    std::shared_ptr<std::atomic_bool> finished = std::make_shared<std::atomic_bool>(false);
    std::shared_ptr<std::atomic_bool> myStop = std::make_shared<std::atomic_bool>(false);
    std::shared_ptr<std::promise<mata::nfa::Nfa>> prom = std::make_shared<std::promise<mata::nfa::Nfa>>();
    std::future<mata::nfa::Nfa> fut = prom->get_future();
    std::thread thread(createAutomaton, creator, prom, finished);
    while (true) {
        if (stop != nullptr && stop->load()) {
            thread.detach();
            mata::nfa::Nfa ret;
            return ret;
        }
        if (finished->load()) {
            thread.join();
            break;
        }
        std::this_thread::yield();
    }
    return fut.get();
}

bool SizeBasedEnumerationReplaceUnderApprox::testString(int contextIndex, const std::string &str) {
    mata::Word word;
    for(char c : str) {
        word.push_back(c);
    }
    return levenshteinAutomata.at(contextIndex).is_in_lang(word);
}

SizeBasedEnumerationReplaceUnderApprox::SizeBasedEnumerationReplaceUnderApprox(
    const std::shared_ptr<OEValuesManager> &oeManager, const std::shared_ptr<VocabFactory> &vocabFactory,
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &expectedOutputs,
    std::atomic<bool> *stop) : SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>(
        oeManager, vocabFactory, contexts, expectedOutputs, stop) {
    radius = Config::GetInstance()->getLevenshteinRadius();
    addSpecialLeafMakers();
    alphabetPerContext.reserve(contexts.size());
    levenshteinAutomata.reserve(contexts.size());
    //for (int i = 0; i < contexts.size(); i++) {
    //    alphabetPerContext.push_back(constructAlphabet(i));
    //    levenshteinAutomata.push_back(createNfaForContext(i));
    //}
    replBottomUpDeduction = std::make_unique<ReplBottomUpDeduction>(this->contexts, this->expectedOutputs, this->expectedOutputsVals, vocabFactory);
}


std::set<char> SizeBasedEnumerationReplaceUnderApprox::constructAlphabet(int contextIndex) {
    std::set<char> ret = getAlphabetConstants();
    for(std::pair<const std::string, variant > var: contexts.at(contextIndex)) {
        if(Helper::typeOf(var.second) != Helper::Type::String) continue;
        std::string val = std::get<std::string>(var.second);
         for(char c : val) {
            ret.insert(c);
        }
    }
    return ret;
}

std::set<char> SizeBasedEnumerationReplaceUnderApprox::getAlphabetConstants() {
    std::set<char> ret;
    for(const std::shared_ptr<VocabMaker> &vocabMaker: vocabFactory->getLeaves()) {
        if(vocabMaker->isVar()) continue;
        std::string val = vocabMaker->getHead();
        for(char c : val) {
            ret.insert(c);
        }
    }
    return ret;
}

std::optional<ASTNode *> SizeBasedEnumerationReplaceUnderApprox::doBottomUpDeduction(ASTNode *newProg) {
    return replBottomUpDeduction->doBottomUpDeduction(newProg);
}

bool SizeBasedEnumerationReplaceUnderApprox::getViolation(ASTNode *newProg) {
    if(newProg->getNodeType().getType() != Helper::Type::String) return false;
    const std::vector<variant> &vals = newProg->getValues();
    std::vector<std::string> valStrings;
    valStrings.reserve(vals.size());
    for(const variant& val: vals) {
        valStrings.push_back(std::get<std::string>(val));
    }
    if (!constructedLevenshteinAutomata) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // prevents errors in unsafe function above
        alphabetPerContext.reserve(contexts.size());
        levenshteinAutomata.reserve(contexts.size());
        for (int i = 0; i < contexts.size(); i++) {
            alphabetPerContext.push_back(constructAlphabet(i));
            if (stop != nullptr && stop->load()) {
                return false;
            }
            levenshteinAutomata.push_back(createNfaForContext(i));
        }
        constructedLevenshteinAutomata = true;
    }
    for(int i = 0; i < valStrings.size(); i++) {
        if(!testString(i, valStrings.at(i))) return true;
    }
    return false;
}

void SizeBasedEnumerationReplaceUnderApprox::resetEnumeration() {
    SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>::resetEnumeration();
    //replBottomUpDeduction->clear();
    alphabetPerContext.clear();
    levenshteinAutomata.clear();
    constructedLevenshteinAutomata = false;
    replBottomUpDeduction = std::make_unique<ReplBottomUpDeduction>(this->contexts, this->expectedOutputs, this->expectedOutputsVals, vocabFactory);
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumerationReplaceUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpReplaceUnderApprox);
}
