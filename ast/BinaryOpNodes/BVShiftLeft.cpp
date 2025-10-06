
#include "BVShiftLeft.h"

BVShiftLeft::BVShiftLeft(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvshl";
    initValues();
}

uint64_t BVShiftLeft::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVShiftLeft::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    if(right >= 64ULL || right < 0ULL) return 0;
    return left << right;
}
