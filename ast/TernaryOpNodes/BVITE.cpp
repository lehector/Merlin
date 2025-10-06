
#include "BVITE.h"

BVITE::BVITE(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "ite";
    initValues();
}

uint64_t BVITE::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

uint64_t BVITE::doOpStatic(variant a0, variant a1, variant a2) {
    bool cond = std::get<bool>(a0);
    uint64_t s1 = std::get<uint64_t>(a1);
    uint64_t s2 = std::get<uint64_t>(a2);
    return cond ? s1 : s2;
}
