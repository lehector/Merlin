
#ifndef SIZEBASEDCOMBINEDBOTTOMUP_H
#define SIZEBASEDCOMBINEDBOTTOMUP_H
#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/BottomUpDeduction.h"


class SizeBasedCombinedBottomUp : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<> {
    /**
     * all bottom-up deductions to use
     */
    std::vector<std::unique_ptr<BottomUpDeduction>> bottomUpDeductions;
public:
    SizeBasedCombinedBottomUp(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedCombinedBottomUp(oeManager,
        vocabFactory, contexts,
        expectedOutputs, nullptr){};

    SizeBasedCombinedBottomUp(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

protected:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    bool getViolation(ASTNode *newProg) override;

    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

    void resetEnumeration() override;
};



#endif //SIZEBASEDCOMBINEDBOTTOMUP_H
