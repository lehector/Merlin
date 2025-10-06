
#ifndef MYSOLVER_BVUREM_H
#define MYSOLVER_BVUREM_H


#include "../BinaryOpNode.h"

class BVURem : public BinaryOpNode<uint64_t>{
public:
    BVURem(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVUREM_H
