
#include "IntEquals.h"

IntEquals::IntEquals(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "=";
    initValues();
}

bool IntEquals::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool IntEquals::doOpStatic(variant l, variant r) {
    return std::get<int>(l) == std::get<int>(r);
}
