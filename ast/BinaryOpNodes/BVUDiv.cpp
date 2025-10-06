
#include "BVUDiv.h"

BVUDiv::BVUDiv(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvudiv";
    initValues();
}

uint64_t BVUDiv::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVUDiv::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    if(right == (uint64_t) 0) return 0xffffffffffffffff;
    return left / right;
}
