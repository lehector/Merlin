
#include "ConstPos.h"

int ConstPos::doOp(variant input) const {
    return doOpStatic(input);
}

ConstPos::ConstPos(ASTNode*arg, const NonTerminal &type) : UnaryOpNode(arg, type){
    head = "ConstPos";
    initValues();
}

int ConstPos::doOpStatic(variant input) {
    return std::get<int>(input);
}
