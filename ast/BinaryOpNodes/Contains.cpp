
#include "Contains.h"

Contains::Contains(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "str.contains";
    initValues();
}

bool Contains::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool Contains::doOpStatic(variant l, variant r) {
    std::string haystack = std::get<std::string>(l);
    std::string needle = std::get<std::string>(r);
    return haystack.contains(needle);
}
