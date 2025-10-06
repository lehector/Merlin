
#ifndef MYSOLVER_INTLITERAL_H
#define MYSOLVER_INTLITERAL_H


#include "../LiteralNode.h"

class IntLiteral : public LiteralNode<int>{
public:
    IntLiteral(int numContexts, int value, const NonTerminal& type);
};


#endif //MYSOLVER_INTLITERAL_H
