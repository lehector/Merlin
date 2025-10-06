
#include "SizeBasedEnumerator.h"

#include <boost/log/trivial.hpp>

SizeBasedEnumerator::SizeBasedEnumerator(const std::shared_ptr<OEValuesManager>& oeManager, const std::shared_ptr<VocabFactory>& vocabFactory,
                                         const std::vector<std::map<std::string, variant>> &contexts) : Enumerator(contexts, oeManager),
                                    vocabFactory(vocabFactory), useOe(Config::GetInstance()->isOePruning()) {
    currIter = vocabFactory->getLeaves();
    boost::unordered_map<int,std::vector<ASTNode*>> childrenBank;
    std::vector<NonTerminal> childrenTypes;
    childrenIterator = std::make_unique<SizeBasedChildrenIterator<ASTNode*>>(childrenBank, childrenTypes, 0);
    rootMaker = currIter.begin();
}

void SizeBasedEnumerator::resetEnumeration() {
    size = 0;
    currIter = vocabFactory->getLeaves();
    rootMaker = currIter.begin();
    clearProgramBank();
    //programBank.clear();
    oeManager->clear();
    nextProg.reset();
    boost::unordered_map<int,std::vector<ASTNode*>> childrenBank = {};
    std::vector<NonTerminal> childrenTypes;
    childrenIterator = std::make_unique<SizeBasedChildrenIterator<ASTNode*>>(childrenBank, childrenTypes, 0);
}

bool SizeBasedEnumerator::advanceRoot() {
    rootMaker++;
    if(rootMaker == currIter.end()) return false;
    // childrenIterator = *rootMaker.getArity() == 0?
    bool isCommutative = false;
    const std::string& head = (*rootMaker)->getHead();
    if(head == "bvand" || head == "bvor" || head == "bvxor" || head == "bvadd" || head == "bvmul" || head == "+") isCommutative = true;
    childrenIterator = std::make_unique<SizeBasedChildrenIterator<ASTNode*>>(programBank, (*rootMaker)->getChildTypes(), size, isCommutative);
    return true;
}

bool SizeBasedEnumerator::changeLevel() {
    if(programBank.empty()) return false;
    currIter = vocabFactory->getNonLeaves();
    rootMaker = currIter.begin();
    size++;
    childrenIterator = std::make_unique<SizeBasedChildrenIterator<ASTNode*>>(programBank, (*rootMaker)->getChildTypes(), size);
    return true;
}

void SizeBasedEnumerator::clearProgramBank() {
    for (const auto& [progSize, astNodes] : programBank) {
        for (ASTNode* astNode : astNodes) {
            delete astNode;
        }
    }
    programBank.clear();
}

void SizeBasedEnumerator::setNextProgram() {
    nextProg.reset();
    while(!nextProg.has_value()){
        if(childrenIterator->hasNext()) {
            std::vector<ASTNode*> children = childrenIterator->getNext();
            if ((*rootMaker)->canMake(children)) {
                ASTNode* newProg = (*rootMaker)->apply(children, contexts);
                newProg->setMaker(*rootMaker);
                if (!useOe || checkIsNewProgram(newProg)) {
                    totalProgsEnumerated++;
                    nextProg = newProg;
                }
                else {
                    delete newProg;
                }
            }
        }
        else if(!advanceRoot()) { // try different root
            if(!changeLevel()) { // else try different level
                return; // nothing works --> return
            }
        }
    }
    addProgramToProgramBank(nextProg.value());
}

void SizeBasedEnumerator::addProgramToProgramBank(ASTNode*nextProg) {
    //assert(nextProg->getTerms() == size + 1);
    programBank[nextProg->getTerms()].push_back(nextProg);
}
