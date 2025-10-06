
#include "ReplBottomUpDeduction.h"

#include "../ast/TernaryOpNodes/StringReplace.h"

ReplBottomUpDeduction::ReplBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
                                             const std::vector<variant> &expected_outputs,
                                             const std::vector<std::string> &expected_outputs_vals,
                                             const std::shared_ptr<VocabFactory> &
                                             vocabFactory): BottomUpDeductionString(
    contexts, expected_outputs, expected_outputs_vals), firstNValuesCheck(Config::GetInstance()->getFirstNValuesCheck()), docomplete(Config::GetInstance()->isReplComplete()) {
    for(const std::shared_ptr<VocabMaker> &maker: vocabFactory->getNonLeaves()) {
        if(maker->getHead() == "str.replace") {
            replMaker = maker;
            noMaker = false;
        }
    }

}

std::vector<std::string> ReplBottomUpDeduction::convertVariantVecToStringVec(const std::vector<variant>& variantVec) {
    std::vector<std::string> result;
    result.reserve(variantVec.size());
    for (const variant& variant : variantVec) {
        result.push_back(std::get<std::string>(variant));
    }
    return result;
}

bool ReplBottomUpDeduction::stringsContainStrings(const std::vector<std::string> &stringsA, const std::vector<std::string> &stringsB) {
    int min = stringsB.size() < firstNValuesCheck ? stringsB.size() : firstNValuesCheck;
    for (int i = 0; i < min; i++) {
        if (stringsA.at(i).contains(stringsB.at(i))) {
            return true;
        }
    }
    return false;
}

bool ReplBottomUpDeduction::stringsDoNotContainStrings(const std::vector<std::string> &stringsA, const std::vector<std::string> &stringsB) {
    int min = stringsB.size() < firstNValuesCheck ? stringsB.size() : firstNValuesCheck;
    for (int i = 0; i < min; i++) {
        if (!stringsA.at(i).contains(stringsB.at(i))) {
            return true;
        }
    }
    return false;
}

std::optional<ASTNode *> ReplBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if (noMaker) return std::nullopt;
    if(newProg->getNodeType().getType() != Helper::Type::String) return std::nullopt;
    //std::string firstVal = std::get<std::string>(newProg->getValues().at(0));
    std::vector<std::string> vals = convertVariantVecToStringVec(newProg->getValues());
    stringAstNodeSet.emplace_back(vals, newProg);
    assert(newProg->getValues().size() >= 1);
    bool checkCCandidate = false;
    if(stringsContainStrings(expectedOutputsVals,vals)) {
        outSubstringAstNodeSet.emplace_back(vals, newProg);
        checkCCandidate = true;
    }
    bool checkBCandidate = false;
    if (!stringsContainStrings(expectedOutputsVals, vals)) {
        checkBCandidate = true;
    }

    // check A candidate
    for(const std::pair<std::vector<std::string>, ASTNode *> &bCandidate: stringAstNodeSet) {
        if(!stringsContainStrings(vals, bCandidate.first)) continue;
        for(const std::pair<std::vector<std::string>, ASTNode*>& cCandidate : outSubstringAstNodeSet) {
            int min = vals.size() < firstNValuesCheck ? vals.size() : firstNValuesCheck;
            bool violated = false;
            for (int i = 0; i < min; i++) {
                if(vals.at(i).size() - bCandidate.first.at(i).size() + cCandidate.first.at(i).size() != expectedOutputsVals.at(i).size()) {
                    violated = true;
                    break;
                }
            }
            if (violated) continue;
            ASTNode *prog = replMaker->apply({newProg, bCandidate.second, cCandidate.second}, contexts);
            prog->setMaker(replMaker);
            if(prog->getValues() == expectedOutputs) return prog;
            delete prog;
        }
    }
    if (!docomplete) return std::nullopt;
    // check B Candidate
    if (checkBCandidate) {
        for(const std::pair<std::vector<std::string>, ASTNode *> &aCandidate: stringAstNodeSet) {
            if(!stringsContainStrings(aCandidate.first, vals)) continue;
            for(const std::pair<std::vector<std::string>, ASTNode*>& cCandidate : outSubstringAstNodeSet) {
                int min = vals.size() < firstNValuesCheck ? vals.size() : firstNValuesCheck;
                bool violated = false;
                for (int i = 0; i < min; i++) {
                    if(aCandidate.first.at(i).size() - vals.at(i).size() + cCandidate.first.at(i).size() != expectedOutputsVals.at(i).size()){
                        violated = true;
                        break;
                    }
                }
                if (violated) continue;
                ASTNode *prog = replMaker->apply({aCandidate.second, newProg, cCandidate.second}, contexts);
                prog->setMaker(replMaker);
                if(prog->getValues() == expectedOutputs) return prog;
                delete prog;
            }
        }
    }

    // check C Candidate
    if (checkCCandidate) {
        for(const std::pair<std::vector<std::string>, ASTNode *> &aCandidate: stringAstNodeSet) {
            for(const std::pair<std::vector<std::string>, ASTNode*>& bCandidate : stringAstNodeSet) {
                if(!stringsContainStrings(aCandidate.first, bCandidate.first)) continue;
                int min = vals.size() < firstNValuesCheck ? vals.size() : firstNValuesCheck;
                bool violated = false;
                for (int i = 0; i < min; i++) {
                    if(aCandidate.first.at(i).size() - bCandidate.first.at(i).size() + vals.at(i).size() != expectedOutputsVals.at(i).size()){
                        violated = true;
                        break;
                    }
                }
                if (violated) continue;
                ASTNode *prog = replMaker->apply({aCandidate.second, bCandidate.second, newProg}, contexts);
                prog->setMaker(replMaker);
                if(prog->getValues() == expectedOutputs) return prog;
                delete prog;
            }
        }
    }

    return std::nullopt;
}

void ReplBottomUpDeduction::clear() {
    stringAstNodeSet.clear();
    outSubstringAstNodeSet.clear();
}
