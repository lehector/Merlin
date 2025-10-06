
#ifndef MYSOLVER_LOR_H
#define MYSOLVER_LOR_H


#include "../BinaryOpNode.h"

class LOr : public BinaryOpNode<bool>{
public:
    LOr(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static bool doOpStatic(variant l, variant r);

private:
    bool doOp(variant l, variant r) const override;

};


#endif //MYSOLVER_LOR_H
