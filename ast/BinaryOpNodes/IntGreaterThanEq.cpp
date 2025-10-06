
#include "IntGreaterThanEq.h"

IntGreaterThanEq::IntGreaterThanEq(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = ">=";
    initValues();
}

bool IntGreaterThanEq::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool IntGreaterThanEq::doOpStatic(variant l, variant r) {
    return std::get<int>(l) >= std::get<int>(r);
}
