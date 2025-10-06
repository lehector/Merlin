
#ifndef MYSOLVER_BVUGT_H
#define MYSOLVER_BVUGT_H


#include "../BinaryOpNode.h"

class BVUgt : public BinaryOpNode<bool> {
public:
    BVUgt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVUGT_H
