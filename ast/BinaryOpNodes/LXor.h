
#ifndef MYSOLVER_LXOR_H
#define MYSOLVER_LXOR_H


#include "../BinaryOpNode.h"

class LXor : public BinaryOpNode<bool>{
public:
    LXor(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_LXOR_H
