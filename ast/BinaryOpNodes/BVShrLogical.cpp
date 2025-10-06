
#include "BVShrLogical.h"

BVShrLogical::BVShrLogical(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvlshr";
    initValues();
}

uint64_t BVShrLogical::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVShrLogical::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);

    if(right >= 64) return 0;
    return left >> right;
}
