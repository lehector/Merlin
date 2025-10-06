
#ifndef MYSOLVER_INTADDITION_H
#define MYSOLVER_INTADDITION_H


#include "../BinaryOpNode.h"

class IntAddition : public BinaryOpNode<int>{
public:
    IntAddition(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static int doOpStatic(variant l, variant r);

private:
    int doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_INTADDITION_H
