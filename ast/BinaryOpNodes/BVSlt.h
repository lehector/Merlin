
#ifndef MYSOLVER_BVSLT_H
#define MYSOLVER_BVSLT_H


#include "../BinaryOpNode.h"

class BVSlt : public BinaryOpNode<bool>{
public:
    BVSlt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_BVSLT_H
