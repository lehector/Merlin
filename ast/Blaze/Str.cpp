
#include "Str.h"

Str::Str(ASTNode* arg, const NonTerminal &type) : UnaryOpNode(arg, type) {
    head = "Str";
    initValues();
}

std::string Str::doOpStatic(variant input) {
    return std::get<std::string>(input);
}

std::string Str::doOp(variant input) const {
    return doOpStatic(input);
}
