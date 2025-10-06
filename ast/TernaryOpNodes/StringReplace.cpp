
#include "StringReplace.h"

StringReplace::StringReplace(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "str.replace";
    initValues();
}

std::string StringReplace::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

std::string StringReplace::doOpStatic(variant a0, variant a1, variant a2) {
    std::string s0 = std::get<std::string>(a0);
    std::string s1 = std::get<std::string>(a1);
    std::string s2 = std::get<std::string>(a2);
    size_t start_pos = s0.find(s1);
    if(start_pos != std::string::npos)
        s0.replace(start_pos, s1.length(), s2);
    return s0;
}
