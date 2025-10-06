
#ifndef MYSOLVER_CONTAINS_H
#define MYSOLVER_CONTAINS_H


#include "../BinaryOpNode.h"

class Contains : public BinaryOpNode<bool>{
public:
    Contains(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_CONTAINS_H
