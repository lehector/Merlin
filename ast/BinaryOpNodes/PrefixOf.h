
#ifndef MYSOLVER_PREFIXOF_H
#define MYSOLVER_PREFIXOF_H


#include "../BinaryOpNode.h"

class PrefixOf : public BinaryOpNode<bool>{
public:
    PrefixOf(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;

};


#endif //MYSOLVER_PREFIXOF_H
