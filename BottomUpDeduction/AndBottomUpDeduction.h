
#ifndef ANDBOTTOMUPDEDUCTION_H
#define ANDBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionBV.h"
#include "../VocabFactory.h"


class AndBottomUpDeduction : public BottomUpDeductionBV {
    /**
     * vector to store good and candidates in
     */
    std::vector<ASTNode*> andBucket;
    /**
     * and vocabmaker
     */
    std::shared_ptr<VocabMaker> andMaker = nullptr;
    /**
     * pointer to config singleton
     */
    Config* config;
    /**
     * whether we have the and maker in the grammar
     */
    bool noMaker = true;

public:
    AndBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<uint64_t> &expected_outputs_vals, const std::vector<variant> &expected_outputs, const std::shared_ptr<VocabFactory>& vocab_factory);

private:
    /**
     * searches for program in bucket, such that bitwise and with newprog results in solution
     * @param newProg program for which to find other program with which to compute bitwise and
     * @param valTargetPairs pairs of value of program and output example
     * @param bucket the bucket to traverse
     * @return optional. has value iff there is a program in the bucket with which conjuction with newProg solves synthesis task
     */
    std::optional<ASTNode *> traverseBucket(ASTNode *newProg,
                                            const std::vector<std::pair<uint64_t, uint64_t>> &valTargetPairs, std::vector<ASTNode*> &bucket);

    /**
     * insert program into bucket. only keeps and maximal programs to keep bucket small
     * @param newProg program to insert
     * @param bucket bucket to insert into
     */
    void insertIntoBucket(ASTNode *newProg, std::vector<ASTNode*> &bucket);

public:
    void clear() override;

    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;
};



#endif //ANDBOTTOMUPDEDUCTION_H
