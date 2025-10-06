
#include "ConstStr.h"

std::string ConstStr::doOp(variant input) const {
    return ConstStr::doOpStatic(input);
}

ConstStr::ConstStr(ASTNode* arg, const NonTerminal &type) : UnaryOpNode(arg, type) {
    head = "ConstStr";
    initValues();
}

std::string ConstStr::doOpStatic(variant input) {
    return std::get<std::string>(input);
}
