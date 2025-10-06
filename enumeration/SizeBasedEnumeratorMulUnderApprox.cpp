
#include "SizeBasedEnumeratorMulUnderApprox.h"

#include <vector>
#include <vector>
#include <vector>
#include <vector>

#include "boost/log/trivial.hpp"

SizeBasedEnumeratorMulUnderApprox::SizeBasedEnumeratorMulUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs, std::atomic<bool>* stop) : SizeBasedEnumeratorBottomUpDeductionUnderApprox(
                                                       oeManager, vocabFactory, contexts, expectedOutputs, stop){
    radius = Config::GetInstance()->getMulRadius();
    mulBottomUpDeduction = std::make_unique<MulBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory, stop);
    addSpecialLeafMakers();
}

SizeBasedEnumeratorMulUnderApprox::SizeBasedEnumeratorMulUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs) : SizeBasedEnumeratorMulUnderApprox(
                                                       oeManager, vocabFactory, contexts, expectedOutputs, nullptr){
}

void SizeBasedEnumeratorMulUnderApprox::resetEnumeration() {
    mulBottomUpDeduction->clear();
    SizeBasedEnumeratorBottomUpDeductionUnderApprox::resetEnumeration();
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumeratorMulUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpMulUnderApprox);
}

std::optional<ASTNode*> SizeBasedEnumeratorMulUnderApprox::doBottomUpDeduction(
    ASTNode*newProg) {
    return mulBottomUpDeduction->doBottomUpDeduction(newProg);
}

bool SizeBasedEnumeratorMulUnderApprox::getViolationStatic(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius) {
    if(progType != Helper::Type::BitVec64) return false;
    if (radius == 75) radius = 88;
    else if (radius == 50) radius = 75;
    else if (radius == 25) radius = 63;
    
    int trailingZeroDiff = 0;
    int maxTrailingZeros = 0;
    for(int i = 0; i < expectedOutputs.size(); i++) {
        const auto& val = progOutputs.at(i);
        const auto& target = expectedOutputs.at(i);
        int trailingZerosTarget = Helper::countTrailingZeros(std::get<uint64_t>(target));
        int trailingZerosVal = Helper::countTrailingZeros(std::get<uint64_t>(val));
        if(trailingZerosVal > trailingZerosTarget) {
            return true;
        }
        maxTrailingZeros += trailingZerosTarget + 1;
        trailingZeroDiff += (trailingZerosTarget - trailingZerosVal) + 1;
    }

    //return false;
    int newRadius = (maxTrailingZeros * radius + 99) / 100; // ceil of percentage
    bool ret =  trailingZeroDiff > newRadius;
    return ret;
    //return trailingZeroDiff > (((int) expectedOutputs.size()) * radius);
}

bool SizeBasedEnumeratorMulUnderApprox::getViolation(ASTNode*newProg) {
    return getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}

bool SizeBasedEnumeratorMulUnderApprox::checkPreCondition(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs) {
    for(int i = 0; i < expectedOutputs.size(); i++) {
        const auto& val = progOutputs.at(i);
        const auto& target = expectedOutputs.at(i);
        int trailingZerosTarget = Helper::countTrailingZeros(std::get<uint64_t>(target));
        int trailingZerosVal = Helper::countTrailingZeros(std::get<uint64_t>(val));
        if(trailingZerosVal > trailingZerosTarget) {
            return true;
        }
    }

    return false;
}
