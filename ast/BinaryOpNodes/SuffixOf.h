
#ifndef MYSOLVER_SUFFIXOF_H
#define MYSOLVER_SUFFIXOF_H


#include "../BinaryOpNode.h"

class SuffixOf : public BinaryOpNode<bool>{
public:
    SuffixOf(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_SUFFIXOF_H
