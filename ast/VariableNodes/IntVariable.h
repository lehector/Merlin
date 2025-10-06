
#ifndef MYSOLVER_INTVARIABLE_H
#define MYSOLVER_INTVARIABLE_H


#include "../VariableNode.h"

class IntVariable : public VariableNode<int>{
public:
    IntVariable(const std::vector<std::map<std::string, variant>> &contexts, std::string varName, const NonTerminal& type);
};


#endif //MYSOLVER_INTVARIABLE_H
