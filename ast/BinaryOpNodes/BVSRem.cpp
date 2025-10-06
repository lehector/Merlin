
#include "BVSRem.h"

BVSRem::BVSRem(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvsrem";
    initValues();
}

uint64_t BVSRem::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVSRem::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    int64_t leftSigned = (int64_t) left;
    int64_t rightSigned = (int64_t) right;
    if(rightSigned == (int64_t) 0) return right;
    int64_t res = leftSigned % rightSigned;
    return (uint64_t) res;
}
