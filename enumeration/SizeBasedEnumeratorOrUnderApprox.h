
#ifndef SIZEBASEDENUMERATORORUNDERAPPROX_H
#define SIZEBASEDENUMERATORORUNDERAPPROX_H
#include <vector>
#include <vector>

#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/OrBottomUpDeduction.h"


class SizeBasedEnumeratorOrUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<> {
    /**
     * bottom-up deduction handler
     */
    std::unique_ptr<OrBottomUpDeduction> orBottomUpDeduction;
    /**
     * radius of ball
     */
    int radius;
public:
    SizeBasedEnumeratorOrUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                     const std::shared_ptr<VocabFactory> &vocabFactory,
                                     const std::vector<std::map<std::string, variant>> &contexts,
                                     const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

    SizeBasedEnumeratorOrUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                     const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                     const std::vector<variant> &expectedOutputs);

protected:
    void resetEnumeration() override;

public:
    /**
     *
     * @param progOutputs outputs of program
     * @param expectedOutputs output examples
     * @return true, iff the program outputs do not satisfy the precondition, i.e. they are not bitwise less than or
     * equal to the output examples
     */
    static bool checkPreCondition(const std::vector<variant>& progOutputs,
                                  const std::vector<variant> &expectedOutputs);

    /**
     * checks whether new_val is bitwise geq to old_val
     * @param new_val bitvec
     * @param old_val bitvec
     * @return new_val >= old_val -- bitwise >=
     */
    static bool isBitwiseGreaterThanEq(uint64_t new_val, uint64_t old_val);

    /**
     * checks whether the progOutputs are in the circle with given radius percentage around gt
     * @param progOutputs program outputs
     * @param expectedOutputs outputs examples
     * @param progType value type of program
     * @param radius radius percentage of circle
     * @return true, iff program is outside the circle
     */
    static bool getViolationStatic(const std::vector<variant>& progOutputs,
                                   const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius);
protected:
    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;

    std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) override;

    bool getViolation(ASTNode*newProg) override;
};



#endif //SIZEBASEDENUMERATORORUNDERAPPROX_H
