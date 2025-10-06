
#ifndef MYSOLVER_BVREDOR_H
#define MYSOLVER_BVREDOR_H


#include "../UnaryOpNode.h"

class BVRedor : public UnaryOpNode<bool>{
public:
    explicit BVRedor(ASTNode* arg, const NonTerminal& type);
    static bool doOpStatic(variant input);

private:
    bool doOp(variant input) const override;

};


#endif //MYSOLVER_BVREDOR_H
