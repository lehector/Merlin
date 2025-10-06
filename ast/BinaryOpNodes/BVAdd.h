
#ifndef MYSOLVER_BVADD_H
#define MYSOLVER_BVADD_H


#include "../BinaryOpNode.h"

class BVAdd : public BinaryOpNode<uint64_t>{
public:
    BVAdd(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVADD_H
