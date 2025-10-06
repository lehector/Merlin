
#ifndef MYSOLVER_LNOT_H
#define MYSOLVER_LNOT_H


#include "../UnaryOpNode.h"

class LNot : public UnaryOpNode<bool>{
public:
    explicit LNot(ASTNode* arg, const NonTerminal& type);
    static bool doOpStatic(variant input);

private:
    bool doOp(variant input) const override;
};


#endif //MYSOLVER_LNOT_H
