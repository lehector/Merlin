
#include "OrBottomUpDeduction.h"

#include "../enumeration/SizeBasedEnumeratorOrUnderApprox.h"

OrBottomUpDeduction::OrBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
                                         const std::vector<uint64_t> &expected_outputs_vals, const std::vector<variant> &expected_outputs,
                                         const std::shared_ptr<VocabFactory> &vocab_factory) : BottomUpDeductionBV(contexts, expected_outputs, expected_outputs_vals) {
    config = Config::GetInstance();
    for(const std::shared_ptr<VocabMaker> &maker: vocab_factory->getNonLeaves()) {
        if(maker->getHead() == "bvor") {
            orMaker = maker;
            noMaker = false;
        }
    }
}

std::optional<ASTNode *> OrBottomUpDeduction::traverseBucket(ASTNode *newProg,
    const std::vector<std::pair<uint64_t, uint64_t>> &valTargetPairs) {
    for (ASTNode*prog: orBucket) {
        bool violation = false;
        for(int i = 0; i < valTargetPairs.size(); i++) {
            uint64_t contextCandidate = std::get<uint64_t>(prog->getValues().at(prog->getValues().size() - 1 - i));
            if ((valTargetPairs.at(i).first | contextCandidate) != valTargetPairs.at(i).second) {
                violation = true;
                break;
            };
        }
        if(violation) continue;
        //BOOST_LOG_TRIVIAL(info) << "found prog that works on last " << valTargetPairs.size() << " values with or";
        ASTNode* orProg = orMaker->apply({newProg, prog}, contexts);
        orProg->setMaker(orMaker);
        if (orProg->getValues() == expectedOutputs) {
            return orProg;
        }
        delete orProg;
    }
    return std::nullopt;
}

void OrBottomUpDeduction::insertIntoBucket(ASTNode *newProg) {
        const std::vector<variant> &newProgVals = newProg->getValues();
    auto ne = std::remove_if(orBucket.begin(), orBucket.end(), [newProgVals] (ASTNode* oldProg) {
        const auto& oldProgVals = oldProg->getValues();
        bool alwaysGreaterThanEq = true;
        for(int i = 0; i < newProgVals.size(); i++) {
            const auto& newVal = std::get<uint64_t>(newProgVals.at(i));
            const auto& oldVal = std::get<uint64_t>(oldProgVals.at(i));
            if(!SizeBasedEnumeratorOrUnderApprox::isBitwiseGreaterThanEq(newVal, oldVal)) {
                alwaysGreaterThanEq = false;
                break;
            }
        }
        return alwaysGreaterThanEq;
    });
    orBucket.erase(ne, orBucket.end());
    orBucket.push_back(newProg);
}

void OrBottomUpDeduction::clear() {
    orBucket.clear();
}

std::optional<ASTNode *> OrBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if (noMaker) return std::nullopt;
    if (newProg->getNodeType().getType() == Helper::Type::BitVec64 && !SizeBasedEnumeratorOrUnderApprox::checkPreCondition(newProg->getValues(), expectedOutputs)) {
        //orBucket.push_back(newProg);
        insertIntoBucket(newProg);
        const std::vector<variant> &progValues = newProg->getValues();
        int numContexts = progValues.size();
        int min = std::min(numContexts, config->getFirstNValuesCheck());
        std::vector<std::pair<uint64_t, uint64_t> > valTargetPairs;
        valTargetPairs.reserve(min);
        for (int i = 0; i < min; i++) {
            valTargetPairs.emplace_back(std::get<uint64_t>(progValues.at(progValues.size() - 1 - i)),
                                        expectedOutputsVals.at(expectedOutputsVals.size() - 1 - i));
        }
        return traverseBucket(newProg, valTargetPairs);
    }
    return std::nullopt;
}
