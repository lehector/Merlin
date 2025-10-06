
#include "SuffixOf.h"

SuffixOf::SuffixOf(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "str.suffixof";
    initValues();
}

bool SuffixOf::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool SuffixOf::doOpStatic(variant l, variant r) {
    std::string ending = std::get<std::string>(l);
    std::string value = std::get<std::string>(r);
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
