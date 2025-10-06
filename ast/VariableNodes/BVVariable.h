
#ifndef MYSOLVER_BVVARIABLE_H
#define MYSOLVER_BVVARIABLE_H


#include "../VariableNode.h"

class BVVariable : public VariableNode<uint64_t>{
public:
    BVVariable(const std::vector<std::map<std::string, variant>> &contexts, std::string varName, const NonTerminal& type);
};


#endif //MYSOLVER_BVVARIABLE_H
