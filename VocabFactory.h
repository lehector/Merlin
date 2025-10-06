
#ifndef MYSOLVER_VOCABFACTORY_H
#define MYSOLVER_VOCABFACTORY_H

#include "VocabMaker.h"

class VocabFactory {
    /**
     * leaf vocab makers, i.e. variables and constants
     */
    std::vector<std::shared_ptr<VocabMaker>> leaves;
    /**
     * non leaf vocab makers
     */
    std::vector<std::shared_ptr<VocabMaker>> nonLeaves;
    /**
     * if then else operators
     * serves as a backup. they are also included in nonLeaves, but will be moved out when moveIteOperatorsOutOfNonLeaves is called
     */
    std::vector<std::shared_ptr<VocabMaker>> iteOperators;
    /**
     * boolean operators
     * serves as a backup. they are also included in nonLeaves, but will be moved out when moveBoolOperatorsOutOfNonLeaves is called
     */
    std::vector<std::shared_ptr<VocabMaker>> boolOperators;
    /**
     * extra leaves that were learned during solving
     */
    std::map<Helper::EnumerationMode, std::vector<std::shared_ptr<VocabMaker>>> extraLeaves;
public:
    /**
     *
     * @return leaf vocab makers, i.e. variables and constants
     */
    [[nodiscard]] const std::vector<std::shared_ptr<VocabMaker>> &getLeaves() const;

    /**
     *
     * @return non leaf vocab makers
     */
    [[nodiscard]] const std::vector<std::shared_ptr<VocabMaker>> &getNonLeaves() const;

    /**
     *
     * @param makers vocab makers the factory will consist of
     */
    VocabFactory(const std::vector<std::shared_ptr<VocabMaker>> &makers);

    /**
     *
     * @return ite makers
     */
    const std::vector<std::shared_ptr<VocabMaker>> &getIteOperators() const;

    /**
     * adds learned vocabmaker to extra leaves
     * @param newLeaves learned vocabmaker
     * @param forEnumerator for which enumerator / metric this program was learned
     */
    void addLeaveOperators(const std::vector<std::shared_ptr<VocabMaker>>& newLeaves, Helper::EnumerationMode forEnumerator);

    /**
     *
     * @param forEnumerator enumerator / metric to get the extra leaves for
     * @return extra leaves for specified enumerator / metric
     */
    const std::vector<std::shared_ptr<VocabMaker>>& getExtraLeaves(Helper::EnumerationMode forEnumerator);

    /**
     * moves the ite operators out of the non leaves
     */
    void moveIteOperatorsOutOfNonLeaves();

    /**
     * moves the bool operators out of the non leaves
     */
    void moveBoolOperatorsOutOfNonLeaves();

};

#endif //MYSOLVER_VOCABFACTORY_H
