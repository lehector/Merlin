
#ifndef MYSOLVER_INTITE_H
#define MYSOLVER_INTITE_H


#include "../TernaryOpNode.h"

class IntITE : public TernaryOpNode<int>{
public:
    IntITE(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type);
    static int doOpStatic(variant a0, variant a1, variant a2);

private:
    int doOp(variant a0, variant a1, variant a2) const override;
};


#endif //MYSOLVER_INTITE_H
