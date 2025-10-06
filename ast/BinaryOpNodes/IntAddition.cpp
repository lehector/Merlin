
#include "IntAddition.h"

int IntAddition::doOp(variant l, variant r) const {
    return doOpStatic(l, r);
}

IntAddition::IntAddition(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : BinaryOpNode(lhs, rhs, type) {
    head = "+";
    initValues();
}

int IntAddition::doOpStatic(variant l, variant r) {
    return std::get<int>(l) + std::get<int>(r);
}
