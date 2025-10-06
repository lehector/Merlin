
#include "LAnd.h"

LAnd::LAnd(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "and";
    initValues();
}

bool LAnd::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool LAnd::doOpStatic(variant l, variant r) {
    bool left = std::get<bool>(l);
    bool right = std::get<bool>(r);
    return left && right;
}
