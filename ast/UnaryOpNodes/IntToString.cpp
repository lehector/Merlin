
#include "IntToString.h"

IntToString::IntToString(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "int.to.str";
    initValues();
}

std::string IntToString::doOp(variant input) const {
    return doOpStatic(input);
}

std::string IntToString::doOpStatic(variant input) {
    int val = std::get<int>(input);
    return val >= 0 ? std::to_string(val) : "";
}
