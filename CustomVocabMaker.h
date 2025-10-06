
#ifndef CUSTOMVOCABMAKER_H
#define CUSTOMVOCABMAKER_H
#include "VocabMaker.h"


/**
 * This class is used to define custom vocab makers.
 * Custom Vocab Makers generate a more complex program than applying an operator, or returning a constant / variable.
 * We use them to implement Learning. We can generate a CustomVocabMaker from a reference ASTNode.
 * The result is a vocabMaker returning the reference ASTNode.
 */
class CustomVocabMaker : public VocabMaker {
    /**
     * reference node to copy when generating a program
     */
    ASTNode* referenceAstNode;
public:
    /**
     *
     * @param returnType return nonterminal of vocabmaker
     * @param head the program the vocabmaker represents
     * @param referenceNode reference program the vocabmaker generates
     * @param contexts input examples
     */
    CustomVocabMaker(const NonTerminal &returnType,
                     const std::string &head, ASTNode* referenceNode, const std::vector<std::map<std::string, variant>>& contexts);

    bool canMake(const std::vector<ASTNode *> &children) override;

    ASTNode * apply(const std::vector<ASTNode *> &children,
        const std::vector<std::map<std::string, variant>> &contexts) override;
};



#endif //CUSTOMVOCABMAKER_H
