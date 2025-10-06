
#include "StringConcat.h"

StringConcat::StringConcat(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "str.++";
    initValues();
}

std::string StringConcat::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

std::string StringConcat::doOpStatic(variant l, variant r) {
    return std::get<std::string>(l) + std::get<std::string>(r);
}
