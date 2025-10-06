
#ifndef MYSOLVER_STRINGCONCAT_H
#define MYSOLVER_STRINGCONCAT_H


#include "../BinaryOpNode.h"

class StringConcat : public BinaryOpNode<std::string>{
    std::string doOp(variant l, variant r) const override;

public:
    StringConcat(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type);
    static std::string doOpStatic(variant l, variant r);
};


#endif //MYSOLVER_STRINGCONCAT_H
