
#include "CustomVocabMaker.h"

CustomVocabMaker::CustomVocabMaker(const NonTerminal &returnType,
                                   const std::string &head, ASTNode* referenceNode, const std::vector<std::map<std::string, variant>>& contexts)
    : VocabMaker(0, {}, returnType, head,
                 [](const std::vector<ASTNode *> &, const std::vector<std::map<std::string, variant> > &) {
                     return nullptr; // dummmy instantiation
                 }, true) {
    referenceAstNode = referenceNode;
}

bool CustomVocabMaker::canMake(const std::vector<ASTNode *> &children) {
    return children.size() == 0;
}

ASTNode * CustomVocabMaker::apply(const std::vector<ASTNode *> &children,
    const std::vector<std::map<std::string, variant>> &contexts) {
    auto ret = referenceAstNode->deepCopy(contexts);
    ret->setTerms(1);
    return ret;
}
