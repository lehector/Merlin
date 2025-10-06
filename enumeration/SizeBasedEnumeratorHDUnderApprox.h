
#ifndef SIZEBASEDENUMERATORHDUNDERAPPROX_H
#define SIZEBASEDENUMERATORHDUNDERAPPROX_H
#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"


class SizeBasedEnumeratorHDUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<> {
    /**
     * radius percentage of circle around gt
     */
    int radius;
public:
    SizeBasedEnumeratorHDUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool>* stop);

    SizeBasedEnumeratorHDUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs) {
        radius = Config::GetInstance()->getHdRadius();
    }

    /**
     *
     * @param progOutputs program outputs
     * @param outputs output examples
     * @param nodeType return type of program
     * @param radius radius percentage of ball
     * @return true, iff program lies outside the ball, i.e. its BV-distance is too high
     */
    static bool getViolationStatic(const std::vector<variant>& progOutputs, const std::vector<variant> &outputs, Helper::Type nodeType, int radius);

protected:
    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;
    std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) override;

    static bool isNotImprovement(ASTNode *newProg, const std::vector<variant> &outputs, int progHd);

    bool getViolation(ASTNode*newProg) override;
};

#endif //SIZEBASEDENUMERATORHDUNDERAPPROX_H
