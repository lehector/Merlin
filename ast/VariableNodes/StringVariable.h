
#ifndef MYSOLVER_STRINGVARIABLE_H
#define MYSOLVER_STRINGVARIABLE_H


#include "../VariableNode.h"

class StringVariable : public VariableNode<std::string>{
public:
    StringVariable(const std::vector<std::map<std::string, variant>> &contexts, std::string varName, const NonTerminal& type);
};


#endif //MYSOLVER_STRINGVARIABLE_H
