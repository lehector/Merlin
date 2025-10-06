
#ifndef MYSOLVER_SUBSTRING_H
#define MYSOLVER_SUBSTRING_H


#include "../TernaryOpNode.h"

class Substring : public TernaryOpNode<std::string>{
public:
    Substring(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type);
    static std::string doOpStatic(variant a0, variant a1, variant a2);

private:
    std::string doOp(variant a0, variant a1, variant a2) const override;
};


#endif //MYSOLVER_SUBSTRING_H
