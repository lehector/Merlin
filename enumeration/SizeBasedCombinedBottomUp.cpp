
#include "SizeBasedCombinedBottomUp.h"

#include "../BottomUpDeduction/AndBottomUpDeduction.h"
#include "../BottomUpDeduction/MulBottomUpDeduction.h"
#include "../BottomUpDeduction/OrBottomUpDeduction.h"
#include "../BottomUpDeduction/XorAddBottomUpDeduction.h"

SizeBasedCombinedBottomUp::SizeBasedCombinedBottomUp(const std::shared_ptr<OEValuesManager> &oeManager,
                                                     const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                                     const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop)
        : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, stop) {
    bottomUpDeductions.push_back(std::make_unique<XorAddBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory));
    bottomUpDeductions.push_back(std::make_unique<MulBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory, stop));
    bottomUpDeductions.push_back(std::make_unique<AndBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory));
    bottomUpDeductions.push_back(std::make_unique<OrBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory));
    addSpecialLeafMakers();
}

std::optional<ASTNode *> SizeBasedCombinedBottomUp::doBottomUpDeduction(ASTNode *newProg) {
    for(const std::unique_ptr<BottomUpDeduction> &bottomUpDeducation: bottomUpDeductions) {
        std::optional<ASTNode*> ret = bottomUpDeducation->doBottomUpDeduction(newProg);
        if(ret.has_value()) return ret;
    }
    return std::nullopt;
}

bool SizeBasedCombinedBottomUp::getViolation(ASTNode *newProg) {
    return false;
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedCombinedBottomUp::getSpecialLeafMakers() {
    std::vector<std::shared_ptr<VocabMaker>> ret;
    ret = vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpAndUnderApprox);
    std::vector<std::shared_ptr<VocabMaker> > orMakers = vocabFactory->getExtraLeaves(
        Helper::EnumerationMode::SizeBottomUpOrUnderApprox);
    ret.insert(ret.end(), orMakers.begin(), orMakers.end());
    std::vector<std::shared_ptr<VocabMaker> > mulMakers = vocabFactory->getExtraLeaves(
        Helper::EnumerationMode::SizeBottomUpMulUnderApprox);
    ret.insert(ret.end(), mulMakers.begin(), mulMakers.end());
    return ret;
}

void SizeBasedCombinedBottomUp::resetEnumeration() {
    for(const std::unique_ptr<BottomUpDeduction> &bottomUpDeducation: bottomUpDeductions) {
        bottomUpDeducation->clear();
    }
    SizeBasedEnumeratorBottomUpDeductionUnderApprox::resetEnumeration();
}
