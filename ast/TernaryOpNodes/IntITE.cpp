
#include "IntITE.h"

IntITE::IntITE(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "ite";
    initValues();
}

int IntITE::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

int IntITE::doOpStatic(variant a0, variant a1, variant a2) {
    bool cond = std::get<bool>(a0);
    int s1 = std::get<int>(a1);
    int s2 = std::get<int>(a2);
    return cond ? s1 : s2;
}
