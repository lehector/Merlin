
#ifndef MYSOLVER_INTGREATERTHANEQ_H
#define MYSOLVER_INTGREATERTHANEQ_H

#include "../BinaryOpNode.h"


class IntGreaterThanEq : public BinaryOpNode<bool>{
public:
private:
    bool doOp(variant l, variant r) const override;

public:
    IntGreaterThanEq(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);
};

#endif //MYSOLVER_INTGREATERTHANEQ_H
