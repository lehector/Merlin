
#ifndef MYSOLVER_BVSREM_H
#define MYSOLVER_BVSREM_H


#include "../BinaryOpNode.h"

class BVSRem : public BinaryOpNode<uint64_t>{
public:
    BVSRem(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSREM_H
