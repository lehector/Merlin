#ifndef BOTTOMUPDEDUCTION_H
#define BOTTOMUPDEDUCTION_H
#include "../ast/ASTNode.h"


class BottomUpDeduction {
public:
    /**
     * computes the successor through bottom-up deduction
     * @param newProg program to compute the successor of
     * @return optional. if it has a value, there is a successor through bottom-up deduction. it solves the synthesis problem
     */
    virtual std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) = 0;
protected:
    /**
     * input examples
     */
    const std::vector<std::map<std::string, variant>>& contexts;
    /**
     * output examples
     */
    const std::vector<variant>& expectedOutputs;

public:
    /**
     *
     * @param contexts input examples
     * @param expected_outputs output examples
     */
    BottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
                      const std::vector<variant> &expected_outputs)
        : contexts(contexts),
          expectedOutputs(expected_outputs) {
    }

    /**
     * clear saved programs
     */
    virtual void clear() = 0;
};



#endif //BOTTOMUPDEDUCTION_H
