
#ifndef MYSOLVER_BOOLVARIABLE_H
#define MYSOLVER_BOOLVARIABLE_H


#include "../VariableNode.h"

class BoolVariable : public VariableNode<bool>{
public:
    BoolVariable(const std::vector<std::map<std::string, variant>> &contexts, std::string varName, const NonTerminal& type);
};


#endif //MYSOLVER_BOOLVARIABLE_H
