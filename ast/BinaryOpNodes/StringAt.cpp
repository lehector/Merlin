
#include "StringAt.h"

StringAt::StringAt(ASTNode* lhs, ASTNode* rhs,  const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "str.at";
    initValues();
}

std::string StringAt::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

std::string StringAt::doOpStatic(variant l, variant r) {
    std::string str = std::get<std::string>(l);
    int idx = std::get<int>(r);
    if(idx < 0 || idx >= str.length()) return "";
    return "" + str.substr(idx,1);
}
