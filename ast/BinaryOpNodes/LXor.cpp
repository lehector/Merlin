
#include "LXor.h"

LXor::LXor(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "xor";
    initValues();
}

bool LXor::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool LXor::doOpStatic(variant l, variant r) {
    bool left = std::get<bool>(l);
    bool right = std::get<bool>(r);
    return left ^ right;
}
