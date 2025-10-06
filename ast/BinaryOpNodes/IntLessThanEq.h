
#ifndef MYSOLVER_INTLESSTHANEQ_H
#define MYSOLVER_INTLESSTHANEQ_H


#include "../BinaryOpNode.h"

class IntLessThanEq : public BinaryOpNode<bool>{
private:
    bool doOp(variant l, variant r) const override;

public:
    IntLessThanEq(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);
};


#endif //MYSOLVER_INTLESSTHANEQ_H
