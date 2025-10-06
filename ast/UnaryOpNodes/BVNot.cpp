
#include "BVNot.h"

BVNot::BVNot(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "bvnot";
    initValues();
}

uint64_t BVNot::doOp(variant input) const {
    return doOpStatic(input);
}

uint64_t BVNot::doOpStatic(variant input) {
    uint64_t val = std::get<uint64_t>(input);
    return ~val;
}
