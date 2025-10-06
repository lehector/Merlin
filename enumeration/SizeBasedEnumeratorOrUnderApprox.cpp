
#include "SizeBasedEnumeratorOrUnderApprox.h"
#include "boost/log/trivial.hpp"

SizeBasedEnumeratorOrUnderApprox::SizeBasedEnumeratorOrUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs, std::atomic<bool>* stop) : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, stop) {
    radius = Config::GetInstance()->getAndOrRadius();
    orBottomUpDeduction = std::make_unique<OrBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory);
    addSpecialLeafMakers();
}

SizeBasedEnumeratorOrUnderApprox::SizeBasedEnumeratorOrUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs) : SizeBasedEnumeratorOrUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
}

void SizeBasedEnumeratorOrUnderApprox::resetEnumeration() {
    orBottomUpDeduction->clear();
    SizeBasedEnumeratorBottomUpDeductionUnderApprox::resetEnumeration();
}

bool SizeBasedEnumeratorOrUnderApprox::checkPreCondition(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs) {
    for(int i = 0; i < expectedOutputs.size(); i++) {
        const auto& val = std::get<uint64_t>(progOutputs.at(i));
        const auto& target = std::get<uint64_t>(expectedOutputs.at(i));
        if((~val | target) != 0xffffffffffffffff) { // bitwise less than eq
            return true;
        }
    }
    return false;
}

bool SizeBasedEnumeratorOrUnderApprox::isBitwiseGreaterThanEq(uint64_t a, uint64_t b) {
    return (~b | a) == 0xffffffffffffffff;
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumeratorOrUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpOrUnderApprox);
}

std::optional<ASTNode* > SizeBasedEnumeratorOrUnderApprox::doBottomUpDeduction(
    ASTNode*newProg) {
    return orBottomUpDeduction->doBottomUpDeduction(newProg);
}

bool SizeBasedEnumeratorOrUnderApprox::getViolationStatic(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs, const Helper::Type progType, int radius) {
    if(progType != Helper::Type::BitVec64) return false;
    int hd = 0;
    int sumMaxHd = 0;
    for(int i = 0; i < expectedOutputs.size(); i++) {
        const auto& val = std::get<uint64_t>(progOutputs.at(i));
        const auto& target = std::get<uint64_t>(expectedOutputs.at(i));
        if((~val | target) != 0xffffffffffffffff) { // bitwise less than eq
            return true;
        }
        int maxHd = Helper::hammingDistance(0x0000000000000000, target);
        sumMaxHd += maxHd;
        hd += Helper::hammingDistance(val, target);
    }
    int newRadius = (sumMaxHd * radius + 99) / 100; // ceil of percentage
    return hd > newRadius;
}

bool SizeBasedEnumeratorOrUnderApprox::getViolation(ASTNode*newProg) {
    return getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}
