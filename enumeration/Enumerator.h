
#ifndef MYSOLVER_ENUMERATOR_H
#define MYSOLVER_ENUMERATOR_H

#include "../ast/ASTNode.h"
#include "OEValuesManager.h"

class Enumerator {
protected:
    /**
     * counts amount of progs that were enumerated, as in part of the search space
     */
    uint64_t totalProgsEnumerated = 0;
    /**
     * saves successor program
     */
    std::optional<ASTNode*> nextProg;
    /**
     * input examples
     */
    const std::vector<std::map<std::string, variant>>& contexts;
    /**
     * factorization handler
     */
    std::shared_ptr<OEValuesManager> oeManager;

    /**
     * checks whether newProg is a new program (equivalence class) or if it can be factorized
     * @param newProg prog to check
     * @return true, iff the program is new, i.e. its values have not been seen before
     */
    virtual bool checkIsNewProgram(ASTNode* newProg);

    /**
     * sets the successor program
     */
    virtual void setNextProgram() = 0;

public:
    /**
     *
     * @return number of total programs enumerated
     */
    [[nodiscard]] uint64_t getTotalProgsEnumerated() const;

    /**
     * resets the enumerator, clearing all programs
     */
    virtual void resetEnumeration() = 0;

    /**
     *
     * @param contexts input examples
     * @param oeManager factorization handler
     */
    Enumerator(const std::vector<std::map<std::string, variant>> &contexts,
               const std::shared_ptr<OEValuesManager> &oeManager);

    /**
     *
     * @return true, iff there is a successor program
     */
    bool hasNext();

    /**
     *
     * @return successor program
     */
    ASTNode* next();

    /**
     * adds input example
     * @param example input example
     */
    virtual void addExample(const std::map<std::string, variant>& example);
};


#endif //MYSOLVER_ENUMERATOR_H
