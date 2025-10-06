
#ifndef MYSOLVER_INTEQUALS_H
#define MYSOLVER_INTEQUALS_H


#include "../BinaryOpNode.h"

class IntEquals : public BinaryOpNode<bool>{
public:
    IntEquals(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_INTEQUALS_H
