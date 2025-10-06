
#include "SizeBasedChildrenIterator.h"

template<>
bool SizeBasedChildrenIterator<ASTNode*>::isOfType(ASTNode* child, const NonTerminal &type) {
    return type == child->getNodeType();
}

template<>
bool SizeBasedChildrenIterator<std::shared_ptr<std::vector<variant>>>::isOfType(std::shared_ptr<std::vector<variant>> child, const NonTerminal &type) {
    assert(!child->empty() && "think about this case");
    variant val = child->at(0);
    assert(false && "broke compatibility by changing type to nonterminal");
    return true;
}

template<>
bool SizeBasedChildrenIterator<std::pair<ASTNode*, std::vector<int>>, std::vector<int>>::isOfType(std::pair<ASTNode*, std::vector<int>> child, const std::vector<int> &type) {
    return true; // handled elsewhere
    //return node.second == type;
}