
#include "BVUlt.h"

BVUlt::BVUlt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvult";
    initValues();
}

bool BVUlt::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool BVUlt::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    return left < right;
}
