
#ifndef MYSOLVER_BVULE_H
#define MYSOLVER_BVULE_H

#include "../BinaryOpNode.h"

class BVUle : public BinaryOpNode<bool>{
public:
    BVUle(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVULE_H
