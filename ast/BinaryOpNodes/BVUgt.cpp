
#include "BVUgt.h"

BVUgt::BVUgt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvugt";
    initValues();
}

bool BVUgt::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool BVUgt::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    return left > right;
}
