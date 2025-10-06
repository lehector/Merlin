
#include "SizeBasedEnumeratorXorAdd.h"

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumeratorXorAdd::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpXorAdd);
}

SizeBasedEnumeratorXorAdd::SizeBasedEnumeratorXorAdd(const std::shared_ptr<OEValuesManager> &oeManager,
                                                     const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                                     const std::vector<variant> &expectedOutputs, std::atomic<bool>* stop) : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, stop) {
    xorAddBottomUpDeduction = std::make_unique<XorAddBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory);
    addSpecialLeafMakers();
}

SizeBasedEnumeratorXorAdd::SizeBasedEnumeratorXorAdd(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs) : SizeBasedEnumeratorXorAdd(oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
}

std::optional<ASTNode*> SizeBasedEnumeratorXorAdd::doBottomUpDeduction(
    ASTNode*newProg) {
    return xorAddBottomUpDeduction->doBottomUpDeduction(newProg);
}

void SizeBasedEnumeratorXorAdd::resetEnumeration() {
    xorAddBottomUpDeduction->clear();
    SizeBasedEnumeratorBottomUpDeductionUnderApprox::resetEnumeration();
}

bool SizeBasedEnumeratorXorAdd::getViolation(ASTNode*newProg) {
    return false;
}
