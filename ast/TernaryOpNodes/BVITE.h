
#ifndef MYSOLVER_BVITE_H
#define MYSOLVER_BVITE_H


#include "../TernaryOpNode.h"

class BVITE : public TernaryOpNode<uint64_t>{
public:
    BVITE(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type);
    static uint64_t doOpStatic(variant a0, variant a1, variant a2);

private:
    uint64_t doOp(variant a0, variant a1, variant a2) const override;
};


#endif //MYSOLVER_BVITE_H
