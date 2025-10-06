
#ifndef MYSOLVER_BVEQUALS_H
#define MYSOLVER_BVEQUALS_H


#include "../BinaryOpNode.h"

class BVEquals : public BinaryOpNode<bool>{
public:
    BVEquals(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);
private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVEQUALS_H
