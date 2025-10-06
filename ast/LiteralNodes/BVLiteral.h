
#ifndef MYSOLVER_BVLITERAL_H
#define MYSOLVER_BVLITERAL_H


#include "../LiteralNode.h"

class BVLiteral : public LiteralNode<uint64_t>{
public:
    BVLiteral(int numContexts, uint64_t value, const NonTerminal& type);
};


#endif //MYSOLVER_BVLITERAL_H
