
#ifndef SIZEBASEDENUMERATIONCONCATUNDERAPPROX_H
#define SIZEBASEDENUMERATIONCONCATUNDERAPPROX_H
#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/ConcatBottomUpDeduction.h"


class SizeBasedEnumerationConcatUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string> {
protected:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    /**
     * bottom-up deduction handler
     */
    std::unique_ptr<ConcatBottomUpDeduction> concatBottomUpDeduction;
    /**
     * radius percentage
     */
    int radius;

public:
    SizeBasedEnumerationConcatUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedEnumerationConcatUnderApprox(
            oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
    }

    SizeBasedEnumerationConcatUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

    /**
     * checks whether program lies outside the radius
     * @param progOutputs outputs of program
     * @param expectedOutputs output examples
     * @param progType return type of program
     * @param radius radius percentage
     * @return true, if program is outside the radius
     */
    static bool getViolationStatic(const std::vector<variant>& progOutputs,
                                   const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius);

protected:
    bool getViolation(ASTNode *newProg) override;

    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

    void resetEnumeration() override;
};



#endif //SIZEBASEDENUMERATIONCONCATUNDERAPPROX_H
