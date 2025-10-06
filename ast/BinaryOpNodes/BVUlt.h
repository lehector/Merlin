
#ifndef MYSOLVER_BVULT_H
#define MYSOLVER_BVULT_H


#include "../BinaryOpNode.h"

class BVUlt : public BinaryOpNode<bool>{
public:
    BVUlt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVULT_H
