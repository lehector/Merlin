
#ifndef REPLBOTTOMUPDEDUCTION_H
#define REPLBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionString.h"
#include "../VocabFactory.h"


class ReplBottomUpDeduction : public BottomUpDeductionString {
    /**
     * replace vocab maker
     */
    std::shared_ptr<VocabMaker> replMaker = nullptr;
    /**
     * ast nodes that return a string
     */
    std::vector<std::pair<std::vector<std::string>, ASTNode*>> stringAstNodeSet;
    /**
     * ast nodes that return a substring of the output example
     */
    std::vector<std::pair<std::vector<std::string>, ASTNode*>> outSubstringAstNodeSet; // viable replacements
    /**
     * whether there is a replace maker
     */
    bool noMaker = true;

    /**
     * for how many values we should check the output before constructing the real program
     */
    const int firstNValuesCheck;

    /**
     *
     * @param variantVec vector of variants
     * @return vector of strings, i.e. the variants casted to strings
     */
    std::vector<std::string> convertVariantVecToStringVec(const std::vector<variant>& variantVec);

    /**
     * check for each i whether stringsA[i] contains stringsB[i]
     * @param stringsA
     * @param stringsB
     * @return whether stringsA[i] contains stringsB[i] for all i
     */
    bool stringsContainStrings(const std::vector<std::string> &stringsA, const std::vector<std::string> &stringsB);

    /**
     * check for each i whether stringsA[i] does not contain stringsB[i]
     * @param stringsA
     * @param stringsB
     * @return whether stringsA[i] does not contain stringsB[i] for all i
     */
    bool stringsDoNotContainStrings(const std::vector<std::string> &stringsA, const std::vector<std::string> &stringsB);

    /**
     * whether to do a complete version of this bottom-up deduction
     */
    const bool docomplete;

public:
    ReplBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expected_outputs, const std::vector<std::string> &expected_outputs_vals, const std::shared_ptr<VocabFactory>& vocabFactory);

    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    void clear() override;
};



#endif //REPLBOTTOMUPDEDUCTION_H
