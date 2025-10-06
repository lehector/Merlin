
#ifndef MYSOLVER_BVSHRLOGICAL_H
#define MYSOLVER_BVSHRLOGICAL_H


#include "../BinaryOpNode.h"

class BVShrLogical : public BinaryOpNode<uint64_t>{
public:
    BVShrLogical(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSHRLOGICAL_H
