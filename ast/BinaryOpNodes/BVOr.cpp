
#include "BVOr.h"

BVOr::BVOr(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvor";
    initValues();
}

uint64_t BVOr::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVOr::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    return left | right;
}
