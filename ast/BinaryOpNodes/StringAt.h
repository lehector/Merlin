
#ifndef MYSOLVER_STRINGAT_H
#define MYSOLVER_STRINGAT_H


#include "../BinaryOpNode.h"

class StringAt : public BinaryOpNode<std::string>{
public:
    StringAt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static std::string doOpStatic(variant l, variant r);

private:
    std::string doOp(variant l, variant r) const override;
};


#endif //MYSOLVER_STRINGAT_H
