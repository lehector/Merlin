
#ifndef SIZEBASEDENUMERATIONREPLACEUNDERAPPROX_H
#define SIZEBASEDENUMERATIONREPLACEUNDERAPPROX_H
#include <mata/nfa/nfa.hh>

#include "SizeBasedEnumeratorBottomUpDeductionUnderApprox.h"
#include "../BottomUpDeduction/ReplBottomUpDeduction.h"


class SizeBasedEnumerationReplaceUnderApprox : public SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>{
    /**
     * radius of ball
     */
    int radius;
    /**
     * levenshtein automata to quickly check whether a program is inside the radius
     */
    std::vector<mata::nfa::Nfa> levenshteinAutomata;

    /**
     *
     * @param contextIndex for which input/output example to create automaton
     * @return levenshtein automaton for example at given index
     */
    mata::nfa::Nfa createNfaForContext(int contextIndex);

    /**
     * alphabet for every example
     */
    std::vector<std::set<char>> alphabetPerContext;

    /**
     * test whether the given string is inside radius wrt. output example of given index
     * @param contextIndex output example index
     * @param str string to test
     * @return true, iff given string is inside radius
     */
    bool testString(int contextIndex, const std::string& str);

    /**
     * bottom-up deduction handler
     */
    std::unique_ptr<ReplBottomUpDeduction> replBottomUpDeduction;

    /**
     * flag whether the LevenshteinAutomata were already constructed
     */
    bool constructedLevenshteinAutomata = false;

public:
    SizeBasedEnumerationReplaceUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedEnumerationReplaceUnderApprox(
            oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
    }

    SizeBasedEnumerationReplaceUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop);

private:
    /**
     * constructs the alphabet for given context
     * @param contextIndex context to construct alphabet for
     * @return alphabet containing chars that this context can produce
     */
    std::set<char> constructAlphabet(int contextIndex);

    /**
     * gets available constants and creates alphabet from them
     * @return alphabet for constants
     */
    std::set<char> getAlphabetConstants();
protected:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    bool getViolation(ASTNode *newProg) override;

    void resetEnumeration() override;

    std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() override;
};



#endif //SIZEBASEDENUMERATIONREPLACEUNDERAPPROX_H
