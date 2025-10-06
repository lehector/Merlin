
#ifndef MYSOLVER_ASTNODE_H
#define MYSOLVER_ASTNODE_H


#include "../Helper.h"
#include "vector"
#include "../NonTerminal.h"

class VocabMaker;


/**
 * This abstract class describes an AST node.
 */
class ASTNode {
public:
    void setTerms(int terms);

protected:
    /**
     * Nonterminal of the ASTNode. Also specifies the node's return type.
     */
    NonTerminal nodeType;
    /**
     * Height of the AST ending in this node
     */
    int height;
    /**
     * Size of the AST ending in this node, i.e. number of terms subterms
     */
    int terms;
    /**
     * children of this AST node
     */
    std::vector<ASTNode*> children;
    /**
     * cost of the ast node -- for cost based enumeration
     */
    int cost;
    /**
     * output values of this AST node
     */
    std::vector<variant> values;
    /**
     * the head, i.e. the operator of this AST node.
     */
    std::string head;
    /**
     * pointer to the maker of this AST node
     */
    std::weak_ptr<VocabMaker> maker;
    /**
     * whether recursively delete this node, when deletion is called
     */
    bool recursivelyDelete = false;
    /**
     * whether the maker pointer is set
     */
    bool makerIsSet = false;
public:
    /**
     *
     * @param targets output examples
     * @return trace of program and distance to output examples. e.g. hamming distance for every subprogram
     * only for dev / debugging purposes
     */
    std::string getTrace(const std::vector<variant>& targets);

    /**
     *
     * @param contexts input examples
     * @return creates a copy of the ast node
     */
    ASTNode* deepCopy(const std::vector<std::map<std::string, variant>>& contexts) const;

    void setRecursivelyDelete(bool recursively_delete);

    [[nodiscard]] std::weak_ptr<VocabMaker> getMaker() const;

    void setMaker(const std::shared_ptr<VocabMaker> &maker);


    explicit ASTNode(const NonTerminal &node_type)
        : nodeType(node_type) {
    }

    virtual ~ASTNode();

    /**
     * check whether var is used in prog
     * @param varName var to check
     * @return whether var is used in prog
     */
    virtual bool includes(std::string varName) = 0;

    /**
     *
     * @return string represenation of ast, i.e. program that the ast represents
     */
    std::string getCode() const;

    /**
     *
     * @return computes "real" size, i.e. uses real size of learned programs, instead of 1
     */
    int getRealSize() const;

    /**
     * evaluates the program on the given inputs
     * @param contexts input examples
     * @return output of the program on the given inputs
     */
    [[nodiscard]] virtual std::vector<variant> eval(const std::vector<std::map<std::string, variant>> &contexts) const = 0;

    /**
     *
     * @param contexts input examples
     * @param targets output examples
     * @return pair of output vals and output examples
     */
    std::pair<std::vector<uint64_t>, std::vector<uint64_t>> extractValuesAndTargets(
        const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &targets) const;

    /**
     *
     * @param contexts input example
     * @return generates a vocab maker from this program (astnode)
     */
    std::shared_ptr<VocabMaker> generateVocabMaker(const std::vector<std::map<std::string, variant>> &contexts) const;

    // learning
    std::vector<std::shared_ptr<VocabMaker>> getAndMaximalComponentsMakers(
        const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const;

    std::vector<std::shared_ptr<VocabMaker>> getOrMaximalComponentsMakers(
        const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const;

    std::vector<std::shared_ptr<VocabMaker>> getUseFulFactors(
        const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &targets) const;

    std::vector<std::shared_ptr<VocabMaker>> getCloseProgs(const std::vector<std::map<std::string, variant>> &contexts,
                                                           const std::vector<variant> &targets) const;

    std::vector<std::shared_ptr<VocabMaker>> getUsefulReplacements(
        const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const;

    std::vector<std::shared_ptr<VocabMaker>> getUsefulSubstrings(
        const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const;

    std::vector<std::shared_ptr<VocabMaker>> getUsefulSuperstrings(
        const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const;



    // getters and setters
    NonTerminal getNodeType() const;

    const std::string &getHead() const;

    int getHeight() const;

    int getTerms() const;

    const std::vector<ASTNode*> &getChildren() const;

    int getCost() const;

    const std::vector<variant> &getValues() const;

};



#endif //MYSOLVER_ASTNODE_H
