
#ifndef MYSOLVER_BVUDIV_H
#define MYSOLVER_BVUDIV_H


#include "../BinaryOpNode.h"

class BVUDiv : public BinaryOpNode<uint64_t>{
public:
    BVUDiv(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVUDIV_H
