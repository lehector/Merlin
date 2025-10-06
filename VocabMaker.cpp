#include "VocabMaker.h"
#include <utility>

bool VocabMaker::canMake(const std::vector<ASTNode*>& children) {
    if(children.size() != arity) return false;
    for(int i = 0; i < children.size(); i++) {
        if(children.at(i)->getNodeType() != childTypes.at(i)) return false;
    }
    return true;
}

int VocabMaker::getArity() const {
    return arity;
}

NonTerminal VocabMaker::getReturnType() const {
    return returnType;
}

const std::string& VocabMaker::getHead() const {
    return head;
}

VocabMaker::VocabMaker(int arity, const std::vector<NonTerminal> &childTypes, NonTerminal returnType, std::string head,
    const std::function<ASTNode *(const std::vector<ASTNode *> &, const std::vector<std::map<std::string, variant>> &)>
    &applyFunc, bool isVar)
        : arity(arity), childTypes(childTypes), returnType(returnType), head(std::move(head)), applyFunc(applyFunc), _isVar(isVar) {}

const std::vector<NonTerminal> &VocabMaker::getChildTypes() const {
    return childTypes;
}

ASTNode* VocabMaker::apply(const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts) {
    return applyFunc(std::move(children), std::move(contexts));
}

bool VocabMaker::isVar() const {
    return _isVar;
}

VocabMaker::VocabMaker(const std::shared_ptr<VocabMaker>& vocabMaker) :
    arity(vocabMaker->arity),
    childTypes(vocabMaker->childTypes),
    returnType(vocabMaker->returnType),
    head(vocabMaker->head),
    applyFunc(vocabMaker->applyFunc),
    _isVar(vocabMaker->_isVar) {}
