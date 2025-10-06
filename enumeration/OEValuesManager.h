
#ifndef MYSOLVER_OEVALUESMANAGER_H
#define MYSOLVER_OEVALUESMANAGER_H

#include <boost/functional/hash.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered/unordered_set.hpp>

#include "../ast/ASTNode.h"

class OEValuesManager {
    /**
     * map to handle observational equivalence / factorization
     * NonTerminal -> Value -> ASTNode
     */
    //boost::unordered_map<std::string, boost::unordered_map<std::vector<variant>, ASTNode*>> valueSet;
    boost::unordered_map<std::string, boost::unordered_set<std::vector<variant>>> valueSet;

public:
    /**
     *
     * @param prog program to test
     * @return true, iff program returns a new set of values, i.e. it is the <=-min (first) prog in its equivalence class
     */
    bool isNewProgram(ASTNode* prog);

    /**
     * deletes the OE Manager
     * also frees the programs that are kept in valueSet
     */
    ~OEValuesManager();

    /**
     * frees programs and clears map
     */
    void clear();
};


#endif //MYSOLVER_OEVALUESMANAGER_H
