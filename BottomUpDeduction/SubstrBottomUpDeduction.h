
#ifndef SUBSTRBOTTOMUPDEDUCTION_H
#define SUBSTRBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionString.h"
#include "boost/unordered/unordered_map.hpp"
#include "../VocabFactory.h"


class SubstrBottomUpDeduction : public BottomUpDeductionString {
    /**
     * maker for substring
     */
    std::shared_ptr<VocabMaker> substrMaker = nullptr;
    /**
     * whether substr maker is in grammar
     */
    bool noMaker = true;
    /**
     * programs that return an integer
     */
    boost::unordered_map<std::vector<int>, ASTNode*> intToProgMap;
    /**
     * programs that return an integer of at least the length of the output examples
     */
    std::vector<std::pair<std::vector<int>, ASTNode*>> lengthIntProgSet;
    /**
     * map from starting positions of output examples in superstrings to the program producing the superstring
     */
    boost::unordered_map<std::vector<int>, std::vector<ASTNode*>> intToStringProgsMap;

    /**
     * inserts program into lengthIntProgSet. only keeps most precise programs, i.e. closest to the length of the output examples
     * @param prog program returning integer values
     * @param vals integer values the program returns
     * @return true, iff an insertion happened
     */
    bool insertIntoLengthIntProgSet(ASTNode *prog, const std::vector<int> &vals);

    /**
     * traverses the too long list. ast_node must be a program returning an integer.
     * we check if it suffices as the length argument of susbtr
     * @param ast_node program returning an integer
     * @return optional. if it has value, it solves the synthesis problem
     */
    std::optional<ASTNode *> traverseTooLongList(ASTNode * ast_node);

    /**
     * traverses the programs returning a viable length
     * @param stringProg program returning superstring
     * @param intProg program returning indices
     * @return optional. if it has a value, it solves the synthesis problem
     */
    std::optional<ASTNode *> traverseLengthIntProgSet(ASTNode * stringProg, ASTNode * intProg);

    /**
     * does bottom up deduction for integer program
     * @param newProg program returning integer
     * @return optional. if it has a value, it solves the synthesis problem
     */
    std::optional<ASTNode *> doBottomUpDeductionFromIntProg(ASTNode *newProg);
    /**
    * does bottom up deduction for string program
    * @param newProg program returning a string
    * @return optional. if it has a value, it solves the synthesis problem
    */
    std::optional<ASTNode *> doBottomUpDeductionFromStringProg(ASTNode *newProg);

    /**
     *
     * @param progVals output of a program
     * @return start positions of output examples in progvals. if one output example is not contained in progvals, the optional does not have a value
     */
    std::optional<std::vector<int>> getSubstrStartPositions(const std::vector<variant>& progVals);

    /**
     * length of output examples
     */
    std::vector<int> outputLengths;
    /**
     * pairs of programs where first returns superstrings of the output and second returns the correct starting indices of the output examples
     * only a program with correct length is still required
     */
    std::vector<std::pair<ASTNode*, ASTNode*>> tooLongList;
public:
    SubstrBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expected_outputs, const std::vector<std::string> &expected_outputs_vals, const std::shared_ptr<VocabFactory>& vocabFactory);

    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    void clear() override;

    /**
     *
     * @param a int vector
     * @param b int vector
     * @return  <= per index
     */
    static bool isLessThanEqPerVal(const std::vector<int> &a, const std::vector<int> &b);
};



#endif //SUBSTRBOTTOMUPDEDUCTION_H
