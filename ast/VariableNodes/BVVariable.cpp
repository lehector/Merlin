
#include "BVVariable.h"

#include <utility>

BVVariable::BVVariable(const std::vector<std::map<std::string, variant>> &contexts, std::string varName, const NonTerminal& type) : VariableNode(contexts, type) {
    head = std::move(varName);
    initValues();
}
