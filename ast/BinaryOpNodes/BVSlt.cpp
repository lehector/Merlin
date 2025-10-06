
#include "BVSlt.h"

BVSlt::BVSlt(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "bvslt";
    initValues();
}

bool BVSlt::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool BVSlt::doOpStatic(variant l, variant r) {
    uint64_t left = std::get<uint64_t>(l);
    uint64_t right = std::get<uint64_t>(r);
    int64_t leftSigned = (int64_t) left;
    int64_t rightSigned = (int64_t) right;
    return leftSigned < rightSigned;
}
