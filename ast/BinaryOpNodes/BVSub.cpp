
#include "BVSub.h"

BVSub::BVSub(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvsub";
    initValues();
}

uint64_t BVSub::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

uint64_t BVSub::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    return left - right;
}
