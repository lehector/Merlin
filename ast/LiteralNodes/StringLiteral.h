
#ifndef MYSOLVER_STRINGLITERAL_H
#define MYSOLVER_STRINGLITERAL_H


#include "../LiteralNode.h"

class StringLiteral : public LiteralNode<std::string>{
public:
    explicit StringLiteral(int numContexts, std::string value, const NonTerminal& type);
};


#endif //MYSOLVER_STRINGLITERAL_H
