
#ifndef XORADDBOTTOMUPDEDUCTION_H
#define XORADDBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionBV.h"
#include "../VocabFactory.h"
#include "boost/unordered/unordered_map.hpp"


class XorAddBottomUpDeduction : public BottomUpDeductionBV {
    /**
     * enumerated programs indexed by their output bitvector
     */
    boost::unordered_map<std::vector<uint64_t>, ASTNode*> progBucket;
    /**
     * maker for xor operator
     */
    std::shared_ptr<VocabMaker> xorMaker;
    /**
    * maker for add operator
    */
    std::shared_ptr<VocabMaker> addMaker;
    /**
    * maker for not operator
    */
    std::shared_ptr<VocabMaker> notMaker;
    /**
    * maker for neg operator
    */
    std::shared_ptr<VocabMaker> negMaker;

    // whether the makers are in the grammar
    bool noXorMaker = true;
    bool noAddMaker = true;
    bool noNotMaker = true;
    bool noNegMaker = true;

public:
    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    XorAddBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<uint64_t> &expected_outputs_vals, const std::vector<variant> &expected_outputs, const std::shared_ptr<VocabFactory>& vocabFactory);

    void clear() override;

private:
    /**
     * computes the required output for each sketch
     * @param newProgVals output of program
     * @param lookingForValsAdd required output for add
     * @param lookingForValsXor  required output for xor
     * @param lookingForValsNegAdd  required output for neg add
     * @param lookingForValsNotAdd  required output for not add
     * @param lookingForValsNegXor required output for neg xor
     * @param lookingForValsNotXor required output for not xor
     */
    void fillLookingForVals(const std::vector<uint64_t> &newProgVals, std::vector<uint64_t> &lookingForValsAdd,
                            std::vector<uint64_t> &lookingForValsXor, std::vector<uint64_t> &lookingForValsNegAdd,
                            std::vector<uint64_t> &lookingForValsNotAdd, std::vector<uint64_t> &lookingForValsNegXor,
                            std::vector<uint64_t> &lookingForValsNotXor);

    /**
     *
     * @param newProg program to do bottom-up deduction for
     * @param newProgVals output of program
     * @param lookingForValsAdd required output for add
     * @param lookingForValsXor  required output for xor
     * @param lookingForValsNegAdd  required output for neg add
     * @param lookingForValsNotAdd  required output for not add
     * @param lookingForValsNegXor required output for neg xor
     * @param lookingForValsNotXor required output for not xor
     * @return optional. if it has a value, there is a program with the correct required output.
     */
    std::optional<ASTNode *> findMatchingProg(ASTNode *newProg, const std::vector<uint64_t> &newProgVals,
                                              const std::vector<uint64_t> &lookingForValsAdd,
                                              const std::vector<uint64_t> &lookingForValsXor,
                                              const std::vector<uint64_t> &lookingForValsNegAdd,
                                              const std::vector<uint64_t> &lookingForValsNotAdd,
                                              const std::vector<uint64_t> &lookingForValsNegXor,
                                              const std::vector<uint64_t> &lookingForValsNotXor);
};



#endif //XORADDBOTTOMUPDEDUCTION_H
