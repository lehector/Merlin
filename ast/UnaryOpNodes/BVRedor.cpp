
#include "BVRedor.h"

BVRedor::BVRedor(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "bvredor";
    initValues();
}

bool BVRedor::doOp(variant input) const {
    return doOpStatic(input);
}

bool BVRedor::doOpStatic(variant input) {
    uint64_t val = std::get<uint64_t>(input);
    return val != 0;
}
