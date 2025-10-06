
#include "BVNeg.h"

BVNeg::BVNeg(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "bvneg";
    initValues();
}

uint64_t BVNeg::doOp(variant input) const {
    return doOpStatic(input);
}

uint64_t BVNeg::doOpStatic(variant input) {
    uint64_t val = std::get<uint64_t>(input);
    int64_t valSigned = (int64_t) val;
    int64_t res = -valSigned;
    return (uint64_t) res;
}
