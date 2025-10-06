
#include "StringLength.h"

StringLength::StringLength(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "str.len";
    initValues();
}

int StringLength::doOp(variant input) const {
    return doOpStatic(input);
}

int StringLength::doOpStatic(variant input) {
    std::string val = std::get<std::string>(input);
    return val.length();
}
