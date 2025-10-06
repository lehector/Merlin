
#include "BVShrArithmetic.h"

BVShrArithmetic::BVShrArithmetic(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvashr";
    initValues();
}

uint64_t BVShrArithmetic::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVShrArithmetic::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);

    int64_t leftSigned = (int64_t) left;

    if(right >= 64 && leftSigned < 0) return (uint64_t) 0xffffffffffffffff;
    if(right >= 64 && leftSigned >= 0) return (uint64_t) 0;
    uint64_t out = (uint64_t) (leftSigned >> right);
    return out;
}
