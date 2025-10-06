
#include "LOr.h"

LOr::LOr(ASTNode* lhs, ASTNode* rhs,  const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "or";
    initValues();
}

bool LOr::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool LOr::doOpStatic(variant l, variant r) {
    bool left = std::get<bool>(l);
    bool right = std::get<bool>(r);
    return left || right;
}
