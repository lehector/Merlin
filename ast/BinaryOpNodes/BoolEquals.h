#ifndef MYSOLVER_BOOLEQUALS_H
#define MYSOLVER_BOOLEQUALS_H


#include "../BinaryOpNode.h"

class BoolEquals : public BinaryOpNode<bool> {
public:
    BoolEquals(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);
private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BOOLEQUALS_H
