
#ifndef MYSOLVER_BVNOT_H
#define MYSOLVER_BVNOT_H


#include "../UnaryOpNode.h"

class BVNot : public UnaryOpNode<uint64_t>{
public:
    explicit BVNot(ASTNode* arg, const NonTerminal& type);
    static uint64_t doOpStatic(variant input);

private:
    uint64_t doOp(variant input) const override;
};


#endif //MYSOLVER_BVNOT_H
