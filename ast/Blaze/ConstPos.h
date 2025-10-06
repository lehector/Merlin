
#ifndef CONSTPOS_H
#define CONSTPOS_H

#include "../UnaryOpNode.h"


class ConstPos : public UnaryOpNode<int>{
    int doOp(variant input) const override;

public:
    ConstPos(ASTNode*arg, const NonTerminal &type);
    static int doOpStatic(variant input);
};



#endif //CONSTPOS_H
