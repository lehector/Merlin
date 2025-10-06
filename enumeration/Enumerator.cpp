
#include "Enumerator.h"

bool Enumerator::hasNext() {
    if(nextProg.has_value()) return true;
    setNextProgram();
    return nextProg.has_value();
}

ASTNode* Enumerator::next() {
    ASTNode* out;
    if(!nextProg.has_value()){
        setNextProgram();
    }
    assert(nextProg.has_value());
    out = nextProg.value();
    nextProg.reset();
    return out;
}

void Enumerator::addExample(const std::map<std::string, variant> &example) {
    //contexts.push_back(example);
    resetEnumeration();
}

Enumerator::Enumerator(const std::vector<std::map<std::string, variant>> &contexts,
                       const std::shared_ptr<OEValuesManager> &oeManager) : contexts(contexts), oeManager(oeManager) {}

uint64_t Enumerator::getTotalProgsEnumerated() const {
    return totalProgsEnumerated;
}

bool Enumerator::checkIsNewProgram(ASTNode*newProg) {
    return oeManager->isNewProgram(newProg);
}
