
#ifndef MYSOLVER_LAND_H
#define MYSOLVER_LAND_H


#include "../BinaryOpNode.h"

class LAnd : public BinaryOpNode<bool>{
public:
    LAnd(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_LAND_H
