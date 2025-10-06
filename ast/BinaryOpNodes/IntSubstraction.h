
#ifndef MYSOLVER_INTSUBSTRACTION_H
#define MYSOLVER_INTSUBSTRACTION_H


#include "../BinaryOpNode.h"

class IntSubstraction : public BinaryOpNode<int>{
public:
    IntSubstraction(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static int doOpStatic(variant l, variant r);

private:
    int doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_INTSUBSTRACTION_H
