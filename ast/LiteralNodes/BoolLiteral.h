
#ifndef MYSOLVER_BOOLLITERAL_H
#define MYSOLVER_BOOLLITERAL_H


#include "../LiteralNode.h"

class BoolLiteral : public LiteralNode<bool>{
public:
    BoolLiteral(int numContexts, bool value, const NonTerminal& type);
};


#endif //MYSOLVER_BOOLLITERAL_H
