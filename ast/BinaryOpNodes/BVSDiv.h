
#ifndef MYSOLVER_BVSDIV_H
#define MYSOLVER_BVSDIV_H


#include "../BinaryOpNode.h"

class BVSDiv : public BinaryOpNode<uint64_t> {
public:
    BVSDiv(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSDIV_H
