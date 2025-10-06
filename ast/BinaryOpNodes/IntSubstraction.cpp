
#include "IntSubstraction.h"

IntSubstraction::IntSubstraction(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "-";
    initValues();
}

int IntSubstraction::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

int IntSubstraction::doOpStatic(variant l, variant r) {
    return std::get<int>(l) - std::get<int>(r);
}
