
#ifndef MYSOLVER_STRINGTOINT_H
#define MYSOLVER_STRINGTOINT_H


#include "../UnaryOpNode.h"

class StringToInt : public UnaryOpNode<int>{
public:
    explicit StringToInt(ASTNode* arg, const NonTerminal& type);
    static int doOpStatic(variant input);

private:
    int doOp(variant input) const override;
};


#endif //MYSOLVER_STRINGTOINT_H
