
#include "StringVariable.h"

#include <utility>

StringVariable::StringVariable(const std::vector<std::map<std::string, variant>> &contexts, std::string varName, const NonTerminal& type) : VariableNode(contexts, type) {
    head = std::move(varName);
    initValues();
}
