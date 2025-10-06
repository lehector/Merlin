
#ifndef SIZEBASEDPORTFOLIO_H
#define SIZEBASEDPORTFOLIO_H
#include "Enumerator.h"
#include "../VocabFactory.h"


class SizeBasedPortfolio : public Enumerator {
protected:
    /**
     * output examples
     */
    const std::vector<variant>& expectedOutputs;
    /**
     * Flag to signal threads to stop
     */
    std::atomic<bool> stop;
    /**
     * vocabFactory representing the grammar
     */
    std::shared_ptr<VocabFactory> vocabFactory;
    /**
     * the enumerators that will run concurrently
     */
    std::vector<std::shared_ptr<Enumerator>> enumerators;

public:
    /**
     *
     * @param contexts input examples
     * @param oeManager handler for (observational equivalence) factorization
     * @param expectedOutputs output examples
     * @param vocabFactory vocabFactory representing the grammar
     */
    SizeBasedPortfolio(const std::vector<std::map<std::string, variant>> &contexts,
                       const std::shared_ptr<OEValuesManager> &oeManager, const std::vector<variant>& expectedOutputs,
                       const std::shared_ptr<VocabFactory>& vocabFactory
    );

    /**
     * runs single enumerator -- this is used for creating a thread
     * @param enumerator enumerator to be run
     * @param mtx a mutex that protects the solution field
     * @param stop stop flag, to stop an enumerator during enumeration
     * @param index the index of the enumerator -- for printing information only
     * @param expectedOutputs output examples
     * @param solutionProg reference to solution field
     */
    static void runEnumerator(const std::shared_ptr<Enumerator> &enumerator, std::mutex &mtx, std::atomic<bool> &stop,
                              int index,
                              const std::vector<variant> &expectedOutputs, std::optional<ASTNode *> &solutionProg);

protected:
    void setNextProgram() override;

public:
    void resetEnumeration() override;

    void addExample(const std::map<std::string, variant> &example) override;
};
#endif //SIZEBASEDPORTFOLIO_H
