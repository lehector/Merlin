
#include "BoolEquals.h"

BoolEquals::BoolEquals(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "=";
    initValues();
}

bool BoolEquals::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool BoolEquals::doOpStatic(variant l, variant r) {
    return std::get<bool>(l) == std::get<bool>(r);
}
