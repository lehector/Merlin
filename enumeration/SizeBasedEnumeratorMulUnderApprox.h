
#ifndef SIZEBASEDENUMERATORMULUNDERAPPROX_H
#define SIZEBASEDENUMERATORMULUNDERAPPROX_H
#include <vector>
#include <vector>

#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/MulBottomUpDeduction.h"


class SizeBasedEnumeratorMulUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<>{
    /**
     * bottom-up deduction hanlder
     */
    std::unique_ptr<MulBottomUpDeduction> mulBottomUpDeduction;
    /**
     * radius percentage of ball
     */
    int radius;
public:
    SizeBasedEnumeratorMulUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                      const std::shared_ptr<VocabFactory> &vocabFactory,
                                      const std::vector<std::map<std::string, variant>> &contexts,
                                      const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

    SizeBasedEnumeratorMulUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
                                      const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
                                      const std::vector<variant> &expectedOutputs);

    /**
     *
     * @param progOutputs outputs of program
     * @param expectedOutputs output examples
     * @param progType return type of program
     * @param radius radius percentage
     * @return true, iff program lies outside the ball
     */
    static bool getViolationStatic(const std::vector<variant> & progOutputs,
                                   const std::vector<variant> & expectedOutputs, Helper::Type progType, int radius);

    /**
     *
     * @param progOutputs program outputs
     * @param expectedOutputs output examples
     * @return true, iff precondition is violated
     */
    static bool checkPreCondition(const std::vector<variant>& progOutputs,
                                  const std::vector<variant> &expectedOutputs);
protected:

    void resetEnumeration() override;

    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;
    std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) override;


    bool getViolation(ASTNode*newProg) override;
};



#endif //SIZEBASEDENUMERATORMULUNDERAPPROX_H
