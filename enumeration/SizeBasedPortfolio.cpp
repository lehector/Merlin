
#include "SizeBasedPortfolio.h"

#include "boost/log/trivial.hpp"
#include "thread"

SizeBasedPortfolio::SizeBasedPortfolio(const std::vector<std::map<std::string, variant>> &contexts,
    const std::shared_ptr<OEValuesManager> &oeManager, const std::vector<variant> &expectedOutputs,
    const std::shared_ptr<VocabFactory> &vocabFactory) : Enumerator(contexts, oeManager), expectedOutputs(expectedOutputs), vocabFactory(vocabFactory) {

}

void SizeBasedPortfolio::runEnumerator(const std::shared_ptr<Enumerator>& enumerator, std::mutex& mtx, std::atomic<bool>& stop, int index, const std::vector<variant>& expectedOutputs, std::optional<ASTNode*>& solutionProg) {
    int lastSize = -1;
    while(enumerator->hasNext()) {
        if(stop.load(std::memory_order::relaxed)) return;
        ASTNode* prog = enumerator->next();
        if (prog->getTerms() != lastSize && prog->getTerms() <= lastSize + 2) {
            // not a bottom up output
            if(prog->getTerms() >= 100) return; // we likely wont find a program in this search space
            lastSize = prog->getTerms();
            BOOST_LOG_TRIVIAL(info) << "Thread " << index << " finished Size: " << lastSize - 1;
            BOOST_LOG_TRIVIAL(info) << "Thread " << index << " enumerated " << enumerator->getTotalProgsEnumerated() << " programs.";
        }

        if(prog->getValues() == expectedOutputs) {
            {
                stop.store(true);
                BOOST_LOG_TRIVIAL(info) << "Thread " << index << " enumerated: " << enumerator->getTotalProgsEnumerated();
                BOOST_LOG_TRIVIAL(info) << "Thread " << index << " found program: " << prog->getCode();
                std::lock_guard<std::mutex> lock(mtx);
                solutionProg = prog;
            }
            return;
        }
    }
}

void SizeBasedPortfolio::setNextProgram() {
    if(enumerators.empty()) {
        BOOST_LOG_TRIVIAL(error) << "No enumerators!";
        exit(1);
    }
    std::mutex mtx;
    std::vector<std::thread> threads;
    std::optional<ASTNode*> solutionProg;
    if(contexts.size() == 0) {
        std::vector<ASTNode*> children;
        nextProg =  vocabFactory->getLeaves().at(0)->apply(children, contexts);
        return;
    }
    stop.store(false);
    threads.reserve(enumerators.size());
    for(int i = 0; i < enumerators.size(); i++) {
        threads.emplace_back(runEnumerator, enumerators.at(i), std::ref(mtx), std::ref(stop), i, expectedOutputs, std::ref(solutionProg));
    }
    for(int i = 0; i < threads.size(); i++) {
        BOOST_LOG_TRIVIAL(info) << "Joining Thread " << i;
        threads.at(i).join();
        BOOST_LOG_TRIVIAL(info) << "Joined Thread " << i;
    }
    nextProg = solutionProg;
}

void SizeBasedPortfolio::resetEnumeration() {
    for(auto& enumerator: enumerators) {
        enumerator->resetEnumeration();
    }
    stop.store(false);
}

void SizeBasedPortfolio::addExample(const std::map<std::string, variant> &example) {
    for(auto& enumerator : enumerators) {
        enumerator->addExample(example);
    }
    Enumerator::addExample(example);
}
