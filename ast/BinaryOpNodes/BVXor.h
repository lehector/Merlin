
#ifndef MYSOLVER_BVXOR_H
#define MYSOLVER_BVXOR_H


#include "../BinaryOpNode.h"

class BVXor : public BinaryOpNode<uint64_t>{
public:
    BVXor(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVXOR_H
