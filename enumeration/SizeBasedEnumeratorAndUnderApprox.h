
#ifndef SIZEBASEDENUMERATORANDUNDERAPPROX_H
#define SIZEBASEDENUMERATORANDUNDERAPPROX_H
#include <vector>
#include <vector>

#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/AndBottomUpDeduction.h"


class SizeBasedEnumeratorAndUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<>{
    /**
     * and bottom-up deduction handler
     */
    std::unique_ptr<AndBottomUpDeduction> andBottomUpDeduction;
    /**
     * radius percentage of the ball
     */
    int radius;
public:
    SizeBasedEnumeratorAndUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                      const std::shared_ptr<VocabFactory> &vocabFactory,
                                      const std::vector<std::map<std::string, variant>> &contexts,
                                      const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

    SizeBasedEnumeratorAndUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                      const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                      const std::vector<variant> &expectedOutputs);

    /**
     *
     * @param a bitvec
     * @param b bitvec
     * @return true, iff a is bitwise less than b
     */
    static bool isBitwiseLessThan(uint64_t a, uint64_t b);

    /**
     *
     * @param progOutputs outputs of program
     * @param expectedOutputs example outputs
     * @return true, iff outputs violate precondition, i.e. they are not bitwise greater than or equal to the output examples
     */
    static bool checkPreCondition(const std::vector<variant>& progOutputs,
                                  const std::vector<variant> &expectedOutputs);

    /**
     *
     * @param a bitvec a
     * @param b bitvec b
     * @return true, if a < b, < is bitwise less than
     */
    static bool isBitwiseLessThanEq(uint64_t a, uint64_t b);

    /**
     * checks whether program is outside the ball
     * @param progOutputs outputs of program
     * @param expectedOutputs output examples
     * @param progType return type of program
     * @param radius radius percentage
     * @return true, iff program is outside the ball
     */
    static bool getViolationStatic(const std::vector<variant>& progOutputs,
                                   const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius);

protected:
    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

    std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) override;

    bool getViolation(ASTNode*newProg) override;

    void resetEnumeration() override;
};



#endif //SIZEBASEDENUMERATORANDUNDERAPPROX_H
