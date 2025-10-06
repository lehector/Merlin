
#ifndef CONCATBOTTOMUPDEDUCTION_H
#define CONCATBOTTOMUPDEDUCTION_H
#include "BottomUpDeductionString.h"
#include "boost/unordered/unordered_map.hpp"
#include "../VocabFactory.h"


class ConcatBottomUpDeduction : public BottomUpDeductionString{
    /**
     * pointer to config singelton
     */
    Config* config;
    /**
     * map from first example output to progs with that output for the first example
     */
    boost::unordered_map<std::string, std::vector<ASTNode*>> firstContextToProgsMap;
    /**
     * concat vocabmaker
     */
    std::shared_ptr<VocabMaker> concatMaker = nullptr;
    /**
     * whether there is concat in the grammar
     */
    bool noMaker = true;
public:
    std::optional<ASTNode*> traverseBucket(const std::vector<std::pair<std::string, std::string> > &pairs,
                        ASTNode* newProg,
                        bool start,
                        const boost::unordered::detail::iterator_detail::iterator<boost::unordered::detail::node<
                                std::pair<const std::string, std::vector<ASTNode *> >, void *>,
                            boost::unordered::detail::bucket<boost::unordered::detail::node<std::pair<const std::string,
                                std::vector<ASTNode *> >, void *>, void *> > &it);

    std::optional<ASTNode *> doBottomUpDeduction(ASTNode *newProg) override;

    void clear() override;

    ConcatBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<std::string> &expected_outputs_vals, const std::vector<variant> &expected_outputs,
        const std::shared_ptr<VocabFactory>& vocabFactory);
};



#endif //CONCATBOTTOMUPDEDUCTION_H
