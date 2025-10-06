
#ifndef MYSOLVER_STRINGLENGTH_H
#define MYSOLVER_STRINGLENGTH_H


#include "../UnaryOpNode.h"

class StringLength : public UnaryOpNode<int>{
public:
    explicit StringLength(ASTNode* arg, const NonTerminal& type);
    static int doOpStatic(variant input);

private:
    int doOp(variant input) const override;

};


#endif //MYSOLVER_STRINGLENGTH_H
