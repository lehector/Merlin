
#ifndef MYSOLVER_BVSLE_H
#define MYSOLVER_BVSLE_H


#include "../BinaryOpNode.h"

class BVSle : public BinaryOpNode<bool>{
public:
    BVSle(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSLE_H
