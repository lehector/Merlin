
#ifndef MYSOLVER_BVSUB_H
#define MYSOLVER_BVSUB_H


#include "../BinaryOpNode.h"

class BVSub : public BinaryOpNode<uint64_t>{
public:
    BVSub(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSUB_H
