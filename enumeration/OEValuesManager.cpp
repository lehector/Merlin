
#include "OEValuesManager.h"

#include <boost/log/trivial.hpp>

bool OEValuesManager::isNewProgram(ASTNode* prog) {
    const std::vector<variant>& vals = prog->getValues();
    //auto [_, isNew] = valueSet[prog->getNodeType().getName()].insert({vals, prog});
    auto [_, isNew] = valueSet[prog->getNodeType().getName()].insert(vals);
    return isNew;
}

void OEValuesManager::clear() {
    //for(const auto& pair : valueSet) {
    //    for(const auto& entry : pair.second) {
    //        delete entry.second;
    //    }
    //}
    this->valueSet.clear();
}


OEValuesManager::~OEValuesManager() {
    clear();
}
