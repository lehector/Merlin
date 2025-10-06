
#ifndef MYSOLVER_BVAND_H
#define MYSOLVER_BVAND_H


#include "../BinaryOpNode.h"

class BVAnd : public BinaryOpNode<uint64_t>{
public:
    BVAnd(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVAND_H
