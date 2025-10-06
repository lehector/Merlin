
#include "PrefixOf.h"

PrefixOf::PrefixOf(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "str.prefixof";
    initValues();
}

bool PrefixOf::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

bool PrefixOf::doOpStatic(variant l, variant r) {
    std::string s = std::get<std::string>(l);
    std::string t = std::get<std::string>(r);
    // s is a prefix of t
    return t.compare(0, s.size(), s) == 0;
}
