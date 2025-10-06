
#include "LNot.h"

LNot::LNot(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "not";
    initValues();
}

bool LNot::doOp(variant input) const {
    return doOpStatic(input);
}

bool LNot::doOpStatic(variant input) {
    bool val = std::get<bool>(input);
    return !val;
}
