
#ifndef MYSOLVER_BVSHIFTLEFT_H
#define MYSOLVER_BVSHIFTLEFT_H


#include "../BinaryOpNode.h"

class BVShiftLeft : public BinaryOpNode<uint64_t>{
public:
    BVShiftLeft(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static uint64_t doOpStatic(variant l, variant r);

private:
    uint64_t doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSHIFTLEFT_H
