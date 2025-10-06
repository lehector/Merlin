
#ifndef SIZEBASEDENUMERATIONSUBSTRUNDERAPPROX_H
#define SIZEBASEDENUMERATIONSUBSTRUNDERAPPROX_H
#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/SubstrBottomUpDeduction.h"


class SizeBasedEnumerationSubstrUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>{
protected:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;
    bool getViolation(ASTNode *newProg) override;
    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

    /**
     * bottom-up deduction handler
     */
    std::unique_ptr<SubstrBottomUpDeduction> substrBottomUpDeduction;
    /**
     * radius percentage
     */
    int radius;

public:
    SizeBasedEnumerationSubstrUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedEnumerationSubstrUnderApprox(
            oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
    }

    SizeBasedEnumerationSubstrUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

    static bool getViolationStatic(const std::vector<variant> & progOutputs,
                                   const std::vector<variant> & expectedOutputs, Helper::Type progType, int radius);
protected:
    void resetEnumeration() override;
};



#endif //SIZEBASEDENUMERATIONSUBSTRUNDERAPPROX_H
