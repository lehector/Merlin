
#include "BVURem.h"

BVURem::BVURem(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvurem";
    initValues();
}

uint64_t BVURem::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVURem::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    if(right ==  0) return  left;
    uint64_t res = left % right;
    return (uint64_t) res;
}
