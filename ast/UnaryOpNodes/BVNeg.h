
#ifndef MYSOLVER_BVNEG_H
#define MYSOLVER_BVNEG_H


#include "../UnaryOpNode.h"

class BVNeg : public UnaryOpNode<uint64_t>{
public:
    explicit BVNeg(ASTNode* arg, const NonTerminal& type);
    static uint64_t doOpStatic(variant input);

private:
    uint64_t doOp(variant input) const override;
};


#endif //MYSOLVER_BVNEG_H
