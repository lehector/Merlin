
#ifndef MYSOLVER_VOCABMAKER_H
#define MYSOLVER_VOCABMAKER_H

#include <z3++.h>
#include "Helper.h"
#include "ast/ASTNode.h"
#include "functional"
#include "NonTerminal.h"

class VocabMaker {
    /**
     * arity of the maker
     */
    int arity;
    /**
     * nonterminals of the children
     */
    std::vector<NonTerminal> childTypes;
    /**
     * nonterminal of the maker
     */
    NonTerminal returnType;
    /**
     * name of the maker, e.g. "bvadd" for bvadd maker
     */
    std::string head;

    /**
     * whether this is a variable vocab maker
     */
    bool _isVar;

    /**
     * the apply function to directly apply the effect of the vocabmaker on given children with given contexts
     */
    std::function<ASTNode*(const std::vector<ASTNode*>&, const std::vector<std::map<std::string, variant>> &)> applyFunc;
public:
    explicit VocabMaker(const std::shared_ptr<VocabMaker>& vocabMaker);

    // nodeType
    /**
     *
     * @return whether maker is variable vocab maker
     */
    bool isVar() const;
    /**
     *
     * @return nonterminals of the children
     */
    [[nodiscard]] const std::vector<NonTerminal> &getChildTypes() const;

    /**
     *
     * @return arity of the vocabmaker
     */
    [[nodiscard]] int getArity() const;
    /**
     *
     * @return nonterminal of the vocabmaker
     */
    [[nodiscard]] NonTerminal getReturnType() const;

    /**
     *
     * @return head of the vocab maker, e.g. "bvadd" for a bvadd maker
     */
    [[nodiscard]] const std::string &getHead() const;

    /**
     *
     * @param arity arity
     * @param childTypes nonterminals of children
     * @param returnType nonterminal of maker
     * @param head head of maker, e.g. "bvadd" for a bvadd maker
     * @param applyFunc function to apply on astnodes and contexts
     * @param isVar whether maker is a variable vocab maker
     */
    VocabMaker(int arity, const std::vector<NonTerminal> &childTypes, NonTerminal returnType, std::string head,
               const std::function<ASTNode*(const std::vector<ASTNode*>&,
               const std::vector<std::map<std::string, variant>> &)> &applyFunc,
               bool isVar = false
    );

    /**
     *
     * @param children proposed children to check whether their type matches
     * @return whether maker can make children, i.e. their types match the children types
     */
    virtual bool canMake(const std::vector<ASTNode*>& children);

    /**
     *
     * @param children children to apply maker on
     * @param contexts contexts for initializing program values
     * @return ast node with given children as children and given vocabmaker as head
     */
    virtual ASTNode* apply(const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts);
};

#endif //MYSOLVER_VOCABMAKER_H
