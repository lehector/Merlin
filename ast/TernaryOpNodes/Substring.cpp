
#include "Substring.h"

Substring::Substring(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "str.substr";
    initValues();
}

std::string Substring::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

std::string Substring::doOpStatic(variant a0, variant a1, variant a2) {
    std::string str = std::get<std::string>(a0);
    int i = std::get<int>(a1);
    int n = std::get<int>(a2);

    if(n < 0 || !(0 <= i && i <= str.length())) return "";
    return str.substr(i, n);
}
