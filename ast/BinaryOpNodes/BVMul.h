
#ifndef MYSOLVER_BVMUL_H
#define MYSOLVER_BVMUL_H


#include "../BinaryOpNode.h"

class BVMul : public BinaryOpNode<uint64_t> {
public:
    BVMul(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVMUL_H
