
#ifndef CONCAT_H
#define CONCAT_H
#include "../BinaryOpNode.h"


class Concat : public BinaryOpNode<std::string>{
    std::string doOp(variant str1, variant str2) const override;
public:
    Concat(ASTNode*lhs, ASTNode*rhs, const NonTerminal &type)
        : BinaryOpNode<std::string>(lhs, rhs, type) {
        initValues();
        //head = "Concat";
        head = "str.++";
    }
    static std::string doOpStatic(variant str1, variant str2);
};



#endif //CONCAT_H
