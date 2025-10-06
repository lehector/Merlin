#ifndef MYSOLVER_SIZEBASEDENUMERATOR_H
#define MYSOLVER_SIZEBASEDENUMERATOR_H

#include "../ast/ASTNode.h"
#include "../VocabMaker.h"
#include "OEValuesManager.h"
#include "../VocabFactory.h"
#include "SizeBasedChildrenIterator.h"
#include "Enumerator.h"

class SizeBasedEnumerator : public Enumerator {
protected:
    /**
     * current vocabmakers to cycle through,
     * basically just leaf makers vs non leaf makers
     */
    std::vector<std::shared_ptr<VocabMaker>> currIter;
    /**
     * the childreniterator to handle enumeration of child vectors
     */
    std::unique_ptr<SizeBasedChildrenIterator<ASTNode*>> childrenIterator;
    /**
     * the current vocab maker
     */
    std::vector<std::shared_ptr<VocabMaker>>::iterator rootMaker;
    /**
     * previously iterated programs
     */
    boost::unordered_map<int, std::vector<ASTNode*>> programBank;
    /**
     * size of already generated programs
     */
    int size = 0;
    /**
     * vocabfactory representing the grammar
     */
    std::shared_ptr<VocabFactory> vocabFactory;
    /**
     * whether to use (observational equivalence) factorization
     */
    const bool useOe;

public:
    /**
     *
     * @param oeManager the (observational equivalence) factorization handler
     * @param vocabFactory vocabFactory representing the grammar
     * @param contexts input examples
     */
    SizeBasedEnumerator(const std::shared_ptr<OEValuesManager>& oeManager, const std::shared_ptr<VocabFactory>& vocabFactory,
                        const std::vector<std::map<std::string, variant>> &contexts);

protected:
    void resetEnumeration() override;

    /**
     * switches the rootMaker to the next vocabmaker in the list
     * @return true, if we there is a next vocabmaker in the list
     */
    bool advanceRoot();

    /**
     * resets the rootMaker to the first rootMaker of the non-leaves
     * increases size
     * @return true, if we could change the level
     */
    virtual bool changeLevel();

    void clearProgramBank();

    void setNextProgram() override;

    /**
     *
     * @param nextProg adds program to the set of already enumerated programs
     */
    virtual void addProgramToProgramBank(ASTNode* nextProg);
};


#endif //MYSOLVER_SIZEBASEDENUMERATOR_H
