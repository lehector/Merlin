
#include "StringITE.h"

StringITE::StringITE(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "ite";
    initValues();
}

std::string StringITE::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

std::string StringITE::doOpStatic(variant a0, variant a1, variant a2) {
    bool cond = std::get<bool>(a0);
    std::string s1 = std::get<std::string>(a1);
    std::string s2 = std::get<std::string>(a2);
    return cond ? s1 : s2;
}
