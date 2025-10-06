
#ifndef MYSOLVER_BVSHRARITHMETIC_H
#define MYSOLVER_BVSHRARITHMETIC_H


#include "../BinaryOpNode.h"

class BVShrArithmetic : public BinaryOpNode<uint64_t>{
public:
    BVShrArithmetic(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSHRARITHMETIC_H
