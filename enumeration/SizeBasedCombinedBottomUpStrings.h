
#ifndef SIZEBASEDCOMBINEDBOTTOMUPSTRINGS_H
#define SIZEBASEDCOMBINEDBOTTOMUPSTRINGS_H
#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/BottomUpDeduction.h"

class SizeBasedCombinedBottomUpStrings : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string> {
    /**
     * all bottom-up deductions to use
     */
    std::vector<std::unique_ptr<BottomUpDeduction>> bottomUpDeductions;
public:
    SizeBasedCombinedBottomUpStrings(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedCombinedBottomUpStrings(oeManager,
        vocabFactory, contexts,
        expectedOutputs, nullptr){};

    SizeBasedCombinedBottomUpStrings(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

protected:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    bool getViolation(ASTNode *newProg) override;

    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

    void resetEnumeration() override;
};



#endif //SIZEBASEDCOMBINEDBOTTOMUPSTRINGS_H
