
#ifndef MYSOLVER_STRINGITE_H
#define MYSOLVER_STRINGITE_H


#include "../TernaryOpNode.h"

class StringITE : public TernaryOpNode<std::string>{
public:
    StringITE(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type);
    static std::string doOpStatic(variant a0, variant a1, variant a2);

private:
    std::string doOp(variant a0, variant a1, variant a2) const override;
};


#endif //MYSOLVER_STRINGITE_H
