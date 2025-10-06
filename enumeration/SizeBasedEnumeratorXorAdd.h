
#ifndef SIZEBASEDENUMERATORXORADD_H
#define SIZEBASEDENUMERATORXORADD_H
#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/XorAddBottomUpDeduction.h"


class SizeBasedEnumeratorXorAdd : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<> {
    protected:
    /**
     * bottom-up deduction handler
     */
    std::unique_ptr<XorAddBottomUpDeduction> xorAddBottomUpDeduction;

    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

public:
    SizeBasedEnumeratorXorAdd(const std::shared_ptr<OEValuesManager> &oeManager,
                              const std::shared_ptr<VocabFactory> &vocabFactory,
                              const std::vector<std::map<std::string, variant>> &contexts,
                              const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

    SizeBasedEnumeratorXorAdd(const std::shared_ptr<OEValuesManager>& oeManager, const std::shared_ptr<VocabFactory>& vocabFactory,
                              const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant>& expectedOutputs);

protected:
    std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) override;

    void resetEnumeration() override;

    bool getViolation(ASTNode*newProg) override;
};



#endif //SIZEBASEDENUMERATORXORADD_H
