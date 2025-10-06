
#ifndef MYSOLVER_INTTOSTRING_H
#define MYSOLVER_INTTOSTRING_H


#include "../UnaryOpNode.h"

class IntToString : public UnaryOpNode<std::string>{
public:
    explicit IntToString(ASTNode* arg, const NonTerminal& type);
    static std::string doOpStatic(variant input);

private:
    std::string doOp(variant input) const override;
};


#endif //MYSOLVER_INTTOSTRING_H
