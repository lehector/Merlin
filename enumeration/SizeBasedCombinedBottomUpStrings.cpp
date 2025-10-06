
#include "SizeBasedCombinedBottomUpStrings.h"

#include "../BottomUpDeduction/ConcatBottomUpDeduction.h"
#include "../BottomUpDeduction/ReplBottomUpDeduction.h"
#include "../BottomUpDeduction/SubstrBottomUpDeduction.h"

SizeBasedCombinedBottomUpStrings::SizeBasedCombinedBottomUpStrings(const std::shared_ptr<OEValuesManager> &oeManager,
                                                                   const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                                                   const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop) : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, stop) {
    bottomUpDeductions.push_back(std::make_unique<ConcatBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory));
    bottomUpDeductions.push_back(std::make_unique<ReplBottomUpDeduction>(this->contexts, this->expectedOutputs, this->expectedOutputsVals, vocabFactory));
    bottomUpDeductions.push_back(std::make_unique<SubstrBottomUpDeduction>(this->contexts, this->expectedOutputs, this->expectedOutputsVals, vocabFactory));
    addSpecialLeafMakers();
}

std::optional<ASTNode *> SizeBasedCombinedBottomUpStrings::doBottomUpDeduction(ASTNode *newProg) {
    for(const std::unique_ptr<BottomUpDeduction> &bottomUpDeducation: bottomUpDeductions) {
        std::optional<ASTNode*> ret = bottomUpDeducation->doBottomUpDeduction(newProg);
        if(ret.has_value()) return ret;
    }
    return std::nullopt;
}

bool SizeBasedCombinedBottomUpStrings::getViolation(ASTNode *newProg) {
    return false;
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedCombinedBottomUpStrings::getSpecialLeafMakers() {
    std::vector<std::shared_ptr<VocabMaker>> ret;
    ret = vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpConcatUnderApprox);
    std::vector<std::shared_ptr<VocabMaker>> substrMakers = vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox);
    ret.insert(ret.end(), substrMakers.begin(), substrMakers.end());
    std::vector<std::shared_ptr<VocabMaker>> replMakers = vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpReplaceUnderApprox);
    ret.insert(ret.end(), replMakers.begin(), replMakers.end());
    return ret;
}

void SizeBasedCombinedBottomUpStrings::resetEnumeration() {
    for(const std::unique_ptr<BottomUpDeduction> &bottomUpDeducation: bottomUpDeductions) {
        bottomUpDeducation->clear();
    }
    SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>::resetEnumeration();
}
