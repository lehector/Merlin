
#ifndef SUBSTR_H
#define SUBSTR_H
#include "../TernaryOpNode.h"
#define ERRSTRING "ERRSTRING"


class Substr : public TernaryOpNode<std::string> {
    std::string doOp(variant a0, variant a1, variant a2) const override;

public:
    Substr(ASTNode*arg0, ASTNode*arg1,
        ASTNode*arg2, const NonTerminal &type);
    static std::string doOpStatic(variant a0, variant a1, variant a2);
};



#endif //SUBSTR_H
