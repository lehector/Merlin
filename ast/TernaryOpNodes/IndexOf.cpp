
#include "IndexOf.h"

IndexOf::IndexOf(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "str.indexof";
    initValues();
}

int IndexOf::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

int IndexOf::doOpStatic(variant a0, variant a1, variant a2) {
    std::string haystack = std::get<std::string>(a0);
    std::string needle = std::get<std::string>(a1);
    int n = std::get<int>(a2);

    if(!(0 <= n && n <= haystack.length())) return -1;
    if(needle == "") return n;
    int out = haystack.find(needle, n);
    if (out == std::string::npos) return -1;

    return out;
}
