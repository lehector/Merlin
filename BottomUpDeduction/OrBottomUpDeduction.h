#ifndef ORBOTTOMUPDEDUCTION_H
#define ORBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionBV.h"
#include "../VocabFactory.h"


class OrBottomUpDeduction : public BottomUpDeductionBV {
    /**
     * pointer to config singleton
     */
    Config *config;

    /**
     * subprograms viable for or program
     */
    std::vector<ASTNode *> orBucket;
    /**
     * or vocab maker
     */
    std::shared_ptr<VocabMaker> orMaker = nullptr;
    /**
     * whether there is the or maker
     */
    bool noMaker = true;

public:
    OrBottomUpDeduction(const std::vector<std::map<std::string, variant> > &contexts,
                        const std::vector<uint64_t> &expected_outputs_vals,
                        const std::vector<variant> &expected_outputs,
                        const std::shared_ptr<VocabFactory> &vocab_factory);

private:
    /**
     * traverses the or bucket to find another subprogram with which the disjunction solves the synthesis problem
     * @param newProg program to perform bottom-up deduction with
     * @param valTargetPairs pairs of output of newProg and the output examples
     * @return optional. if it has a value it is the synthesis solution, otherwise there is no solution through bottom-up deduction
     */
    std::optional<ASTNode *> traverseBucket(ASTNode *newProg,
                                            const std::vector<std::pair<uint64_t, uint64_t> > &valTargetPairs);

    /**
     * inserts program into orBucket
     * @param newProg program to insert into bucket
     */
    void insertIntoBucket(ASTNode *newProg);

public:
    void clear() override;

    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;
};


#endif //ORBOTTOMUPDEDUCTION_H
