
#include "IntLessThanEq.h"

IntLessThanEq::IntLessThanEq(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "<=";
    initValues();
}

bool IntLessThanEq::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool IntLessThanEq::doOpStatic(variant l, variant r) {
    return std::get<int>(l) <= std::get<int>(r);
}
