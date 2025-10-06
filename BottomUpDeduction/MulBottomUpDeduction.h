
#ifndef MULBOTTOMUPDEDUCTION_H
#define MULBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionBV.h"
#include "boost/unordered/unordered_map.hpp"
#include "../VocabFactory.h"


class MulBottomUpDeduction : public BottomUpDeductionBV {
    /**
     * pointer to config singleton
     */
    Config* config;
    /**
     * maker for mul programs
     */
    std::shared_ptr<VocabMaker> mulMaker = nullptr;
    /**
     * map from output on first input example to programs with that output on the first input example
     */
    boost::unordered_map<uint64_t, std::vector<ASTNode*>> firstContextValToProgsMap;
    /**
     * stop flag to stop deduction
     */
    std::atomic<bool>* stop;
    /**
     * true iff there is no mul maker in the grammar
     */
    bool noMaker = true;
public:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    MulBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<uint64_t> &expected_outputs_vals, const std::vector<variant> &expected_outputs, const std::shared_ptr<VocabFactory> &vocabFactory, std::atomic<bool> *stop);

    void clear() override;

    /**
     * computes multiplicative inveses to do bottom-up deduction
     * @param newProg program to perform bottom-up deduction for
     * @param valTargetPairs pairs of outputs of newprog and output examples
     * @param trailingZerosFirstContextVal number of trailing zeros for output of newprog on first example
     * @return optional. if it has a value, bottom-up deduction is possible and the value is the solution to the synthesis task
     */
    std::optional<ASTNode *> getProgramUsingMath(ASTNode *newProg,
                                                 const std::vector<std::pair<uint64_t, uint64_t>> &valTargetPairs,
                                                 int trailingZerosFirstContextVal);

    /**
     * uses extended euclidian algorithm to compute multiplicative inverse
     * @param firstContextVal output of prog for first input example
     * @param firstContextTarget first output example
     * @param trailing_zeros_first_context_val number of trailling zeros of output of prog on first example
     * @param stop stop flag
     * @return vector of multiplicative inverses
     */
    static std::vector<uint64_t> getViableFactorsMath(uint64_t firstContextVal, uint64_t firstContextTarget,
                                                      int trailing_zeros_first_context_val, std::atomic<bool> *stop);

    /**
     *
     * @param viableFactors potential factors for multiplication
     * @param stop stop flag
     * @return program candidates for which to check whether they are solutions
     */
    std::vector<ASTNode *> getViablePrograms(const std::vector<uint64_t> &viableFactors, std::atomic<bool> *stop);

    /**
     * checks whether newPorg * prog = output examples
     * @param newProg prog a
     * @param valTargetPairs ouptut of prog a and output examples
     * @param prog prog b
     * @return optional. has value if a * b = output
     */
    std::optional<ASTNode *> checkProg(ASTNode *newProg,
                                       const std::vector<std::pair<uint64_t, uint64_t>> &valTargetPairs,
                                       ASTNode *prog) const;
};



#endif //MULBOTTOMUPDEDUCTION_H
