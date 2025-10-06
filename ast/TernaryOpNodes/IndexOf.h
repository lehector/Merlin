
#ifndef MYSOLVER_INDEXOF_H
#define MYSOLVER_INDEXOF_H


#include "../TernaryOpNode.h"

class IndexOf : public  TernaryOpNode<int>{
public:
    IndexOf(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type);
    static int doOpStatic(variant a0, variant a1, variant a2);

private:
    int doOp(variant a0, variant a1, variant a2) const override;
};


#endif //MYSOLVER_INDEXOF_H
