#ifndef MYSOLVER_BVOR_H
#define MYSOLVER_BVOR_H


#include "../BinaryOpNode.h"

class BVOr : public BinaryOpNode<uint64_t>{
public:
    BVOr(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVOR_H
