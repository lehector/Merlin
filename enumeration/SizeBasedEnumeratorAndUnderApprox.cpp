
#include "SizeBasedEnumeratorAndUnderApprox.h"
#include "boost/log/trivial.hpp"

SizeBasedEnumeratorAndUnderApprox::SizeBasedEnumeratorAndUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs, std::atomic<bool>* stop) : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, stop){
    andBottomUpDeduction = std::make_unique<AndBottomUpDeduction>(this->contexts, expectedOutputsVals, this->expectedOutputs, vocabFactory);
    addSpecialLeafMakers();
    radius = Config::GetInstance()->getAndOrRadius();
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumeratorAndUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpAndUnderApprox);
}

SizeBasedEnumeratorAndUnderApprox::SizeBasedEnumeratorAndUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                                                     const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                                                     const std::vector<variant> &expectedOutputs) : SizeBasedEnumeratorAndUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
}

bool SizeBasedEnumeratorAndUnderApprox::isBitwiseLessThan(uint64_t a, uint64_t b) {
    return (~b | a) != 0xffffffffffffffff;
}

bool SizeBasedEnumeratorAndUnderApprox::isBitwiseLessThanEq(uint64_t a, uint64_t b) {
    return (~a | b) == 0xffffffffffffffff;
}

std::optional<ASTNode*> SizeBasedEnumeratorAndUnderApprox::doBottomUpDeduction(
    ASTNode*newProg) {
    return andBottomUpDeduction->doBottomUpDeduction(newProg);
}

bool SizeBasedEnumeratorAndUnderApprox::checkPreCondition(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs) {
    for(int i = 0; i < expectedOutputs.size(); i++) {
        const auto& val = std::get<uint64_t>(progOutputs.at(i));
        const auto& target = std::get<uint64_t>(expectedOutputs.at(i));
        if(isBitwiseLessThan(val, target)) { // bitwise less than
            return true;
        }
    }
    return false;
}

bool SizeBasedEnumeratorAndUnderApprox::getViolationStatic(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius) {
    if(progType != Helper::Type::BitVec64) return false;
    int hd = 0;
    int sumMaxHd = 0;
    for(int i = 0; i < expectedOutputs.size(); i++) {
        const auto& val = std::get<uint64_t>(progOutputs.at(i));
        const auto& target = std::get<uint64_t>(expectedOutputs.at(i));
        if(isBitwiseLessThan(val, target)) { // bitwise less than
            return true;
        }
        int maxHd = Helper::hammingDistance(0xffffffffffffffff, target);
        sumMaxHd += maxHd;
        hd += Helper::hammingDistance(val, target);
    }
    int newRadius = (sumMaxHd * radius + 99) / 100; // ceil of percentage
    //return hd > radius * expectedOutputs.size();
    return hd > newRadius;
}

bool SizeBasedEnumeratorAndUnderApprox::getViolation(ASTNode*newProg) {
    return getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}

void SizeBasedEnumeratorAndUnderApprox::resetEnumeration() {
    andBottomUpDeduction->clear();
    SizeBasedEnumeratorBottomUpDeductionUnderApprox::resetEnumeration();
}
