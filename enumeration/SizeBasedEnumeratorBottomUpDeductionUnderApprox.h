
#ifndef SIZEBASEDENUMERATORBOTTOMUPDEDUCTION_H
#define SIZEBASEDENUMERATORBOTTOMUPDEDUCTION_H
#include "SizeBasedEnumerator.h"


template <class T = uint64_t>
class SizeBasedEnumeratorBottomUpDeductionUnderApprox : public SizeBasedEnumerator{
protected:
    /**
     * output examples
     */
    const std::vector<variant> &expectedOutputs;
    /**
     * output examples in the correct type
     */
    std::vector<T> expectedOutputsVals;
    /**
     * whether to apply pruning
     */
    bool checkViolation = false;
    /**
     * pointer to the config singleton
     */
    Config* config;
    /**
     * flag to stop iteration -- useful for running enumerators concurrently
     */
    std::atomic<bool>* stop;

    /**
     * size threshold up to which size we do not apply pruning
     */
    const int cutoffSize;

public:
    /**
     *
     * @param oeManager handler for (observational equivalence) factorization
     * @param vocabFactory vocabFactory representing grammar
     * @param contexts input examples
     * @param expectedOutputs output examples
     */
    SizeBasedEnumeratorBottomUpDeductionUnderApprox(const std::shared_ptr<OEValuesManager>& oeManager, const std::shared_ptr<VocabFactory>& vocabFactory,
                                                    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant>& expectedOutputs);

    /**
     *
     * @param oeManager handler for (observational equivalence) factorization
     * @param vocabFactory vocabFactory representing grammar
     * @param contexts input examples
     * @param expectedOutputs output examples
     * @param stop stop flag to handle concurrency
     */
    SizeBasedEnumeratorBottomUpDeductionUnderApprox(const std::shared_ptr<OEValuesManager>& oeManager, const std::shared_ptr<VocabFactory>& vocabFactory,
                                                    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant>& expectedOutputs, std::atomic<bool>* stop);

protected:
    /**
     * tries to do bottom-up deduction
     * @param newProg newly enumerated program
     * @return program optional. iff bottom-up deduction succeeds, the value is set to the synthesis solution
     */
    virtual std::optional<ASTNode*> doBottomUpDeduction(ASTNode*newProg) = 0;

    /**
     * checks whether the program should be pruned because it falls outside the ball
     * @param newProg program to check
     * @return true, iff the program is outside the ball
     */
    virtual bool getViolation(ASTNode*newProg) = 0;

    void setNextProgram() override;

    void resetEnumeration() override;

    /**
     * adds learned programs to leaf makers
     */
    void addSpecialLeafMakers();

    /**
     * gets learned programs
     * @return makers for learned programs
     */
    virtual std::vector<std::shared_ptr<VocabMaker>> getSpecialLeafMakers() = 0;
};

template <class T>
SizeBasedEnumeratorBottomUpDeductionUnderApprox<T>::SizeBasedEnumeratorBottomUpDeductionUnderApprox(
    const std::shared_ptr<OEValuesManager> &oeManager, const std::shared_ptr<VocabFactory> &vocabFactory,
    const std::vector<std::map<std::string, variant> > &contexts,
    const std::vector<variant> &expectedOutputs) : SizeBasedEnumeratorBottomUpDeductionUnderApprox(
    oeManager, vocabFactory, contexts, expectedOutputs, nullptr) {
};

template <class T>
SizeBasedEnumeratorBottomUpDeductionUnderApprox<T>::SizeBasedEnumeratorBottomUpDeductionUnderApprox(
    const std::shared_ptr<OEValuesManager> &oeManager, const std::shared_ptr<VocabFactory> &vocabFactory,
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &expectedOutputs,
    std::atomic<bool>* stop) : SizeBasedEnumerator(oeManager, vocabFactory, contexts), expectedOutputs(expectedOutputs),
        stop(stop), cutoffSize(Config::GetInstance()->getCutoffSize()) {
    for(const variant &expectedOutput: expectedOutputs) {
        expectedOutputsVals.push_back(std::get<T>(expectedOutput));
    }
    config = Config::GetInstance();
    checkViolation = config->isViolation();
}

template <class T>
void SizeBasedEnumeratorBottomUpDeductionUnderApprox<T>::setNextProgram() {
    nextProg.reset();
    while(!nextProg.has_value()){
        if(stop != nullptr && stop->load(std::memory_order::relaxed)) return;
        if(childrenIterator->hasNext()) {
            std::vector<ASTNode*> children = childrenIterator->getNext();
            if ((*rootMaker)->canMake(children)) {
                ASTNode* newProg = (*rootMaker)->apply(children, contexts);
                if (!useOe || checkIsNewProgram(newProg)) {
                    newProg->setMaker(*rootMaker);
                    if(size >= cutoffSize) {
                        if (checkViolation && getViolation(newProg)) {
                            continue;
                        }
                    }
                    totalProgsEnumerated++;
                    nextProg = newProg;
                    std::optional<ASTNode*> bottomUpResult = doBottomUpDeduction(newProg);
                    if(bottomUpResult.has_value()) nextProg = bottomUpResult;
                }
                else {
                    delete newProg;
                }
            }
        }
        else if(!advanceRoot()) { // try different root
            if(!changeLevel()) { // else try different level
                return; // nothing works --> return
            }
        }
    }
    addProgramToProgramBank(nextProg.value());
}
template <class T>
void SizeBasedEnumeratorBottomUpDeductionUnderApprox<T>::resetEnumeration() {
    expectedOutputsVals.clear();
    for(const variant &expectedOutput: expectedOutputs) {
        expectedOutputsVals.push_back(std::get<T>(expectedOutput));
    }
    SizeBasedEnumerator::resetEnumeration();
    addSpecialLeafMakers();
}

template <class T>
void SizeBasedEnumeratorBottomUpDeductionUnderApprox<T>::addSpecialLeafMakers() {
    std::vector<std::shared_ptr<VocabMaker> > specialLeafMakers = getSpecialLeafMakers();
    for(const std::shared_ptr<VocabMaker> &leafMaker: specialLeafMakers) {
        currIter.push_back(leafMaker);
    }
    rootMaker = currIter.begin();
}


#endif //SIZEBASEDENUMERATORBOTTOMUPDEDUCTION_H
