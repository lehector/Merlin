
#include "AndBottomUpDeduction.h"

#include "../enumeration/SizeBasedEnumeratorAndUnderApprox.h"

std::optional<ASTNode *> AndBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if (noMaker) return std::nullopt;
    if (newProg->getNodeType().getType() == Helper::Type::BitVec64 && !SizeBasedEnumeratorAndUnderApprox::checkPreCondition(newProg->getValues(), expectedOutputs)) {
        insertIntoBucket(newProg, andBucket);

        const std::vector<variant> &progValues = newProg->getValues();
        int numContexts = progValues.size();
        int min = std::min(numContexts, config->getFirstNValuesCheck());
        std::vector<std::pair<uint64_t, uint64_t> > valTargetPairs;
        valTargetPairs.reserve(min);
        for (int i = 0; i < min; i++) {
            valTargetPairs.emplace_back(std::get<uint64_t>(progValues.at(progValues.size() - 1 - i)),
                                        expectedOutputsVals.at(expectedOutputsVals.size() - 1 - i));
        }
        return traverseBucket(newProg, valTargetPairs, andBucket);
    }
    return std::nullopt;
}

AndBottomUpDeduction::AndBottomUpDeduction(const std::vector<std::map<std::string, variant> > &contexts,
                                           const std::vector<uint64_t> &expected_outputs_vals,
                                           const std::vector<variant> &expected_outputs,
                                           const std::shared_ptr<VocabFactory>& vocab_factory)
    : BottomUpDeductionBV(contexts, expected_outputs, expected_outputs_vals) {
    config = Config::GetInstance();
    for (const std::shared_ptr<VocabMaker> &maker: vocab_factory->getNonLeaves()) {
        if (maker->getHead() == "bvand") {
            andMaker = maker;
            noMaker = false;
        }
    }
}

std::optional<ASTNode*> AndBottomUpDeduction::traverseBucket(ASTNode*newProg, const std::vector<std::pair<uint64_t, uint64_t>>& valTargetPairs, std::vector<ASTNode*>& bucket) {
    for(ASTNode*prog: bucket) {
        bool violation = false;
        for (int i = 0; i < valTargetPairs.size(); i++) {
            uint64_t contextCandidate = std::get<uint64_t>(prog->getValues().at(prog->getValues().size() - 1 - i));
            if ((valTargetPairs.at(i).first & contextCandidate) != valTargetPairs.at(i).second) {
                violation = true;
                break;
            };
        }
        if (violation) continue;
        //BOOST_LOG_TRIVIAL(info) << "found prog that works on last " << valTargetPairs.size() << " values with and";
        ASTNode* andProg = andMaker->apply({newProg, prog}, contexts);
        andProg->setMaker(andMaker);
        if(andProg->getValues() == expectedOutputs) {
            return andProg;
        }
        delete andProg;
    }
    return std::nullopt;
}

void AndBottomUpDeduction::insertIntoBucket(ASTNode *newProg, std::vector<ASTNode*> &bucket) {
    const std::vector<variant> &newProgVals = newProg->getValues();
    auto ne = std::remove_if(bucket.begin(), bucket.end(), [newProgVals] (ASTNode* oldProg) {
        const auto& oldProgVals = oldProg->getValues();
        bool alwaysLessThanEq = true;
        for(int i = 0; i < newProgVals.size(); i++) {
            const auto& newVal = std::get<uint64_t>(newProgVals.at(i));
            const auto& oldVal = std::get<uint64_t>(oldProgVals.at(i));
            if(!SizeBasedEnumeratorAndUnderApprox::isBitwiseLessThanEq(newVal, oldVal)) {
                alwaysLessThanEq = false;
                break;
            }
        }
        return alwaysLessThanEq;
    });
    bucket.erase(ne, bucket.end());
    bucket.push_back(newProg);
}

void AndBottomUpDeduction::clear() {
    andBucket.clear();
}
