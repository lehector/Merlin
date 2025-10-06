
#include "XorAddBottomUpDeduction.h"


std::optional<ASTNode *> XorAddBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if(newProg->getNodeType().getType() == Helper::Type::BitVec64) {
        std::vector<uint64_t> newProgVals;
        for (const variant &output: newProg->getValues()) {
            newProgVals.push_back(std::get<uint64_t>(output));
        }
        progBucket[newProgVals] = newProg;

        std::vector<uint64_t> lookingForValsAdd;
        std::vector<uint64_t> lookingForValsXor;
        std::vector<uint64_t> lookingForValsNegAdd;
        std::vector<uint64_t> lookingForValsNotAdd;
        std::vector<uint64_t> lookingForValsNegXor;
        std::vector<uint64_t> lookingForValsNotXor;
        fillLookingForVals(newProgVals, lookingForValsAdd, lookingForValsXor, lookingForValsNegAdd, lookingForValsNotAdd, lookingForValsNegXor, lookingForValsNotXor);

        return findMatchingProg(newProg, newProgVals, lookingForValsAdd, lookingForValsXor, lookingForValsNegAdd, lookingForValsNotAdd, lookingForValsNegXor, lookingForValsNotXor);
    }
    return std::nullopt;
}

XorAddBottomUpDeduction::XorAddBottomUpDeduction(const std::vector<std::map<std::string, variant> > &contexts,
                                                 const std::vector<uint64_t> &expected_outputs_vals,
                                                 const std::vector<variant> &expected_outputs,
                                                 const std::shared_ptr<VocabFactory>& vocabFactory) : BottomUpDeductionBV(
    contexts, expected_outputs, expected_outputs_vals) {
    for(const std::shared_ptr<VocabMaker> &maker: vocabFactory->getNonLeaves()) {
        if(maker->getHead() == "bvxor") {
            xorMaker = maker;
            noXorMaker = false;
        }
        if(maker->getHead() == "bvadd") {
            addMaker = maker;
            noAddMaker = false;
        }
        if(maker->getHead() == "bvneg") {
            negMaker = maker;
            noNegMaker = false;
        }
        if(maker->getHead() == "bvnot") {
            notMaker = maker;
            noNotMaker = false;
        }
    }
}

void XorAddBottomUpDeduction::clear() {
    progBucket.clear();
}

void XorAddBottomUpDeduction::fillLookingForVals(const std::vector<uint64_t>& newProgVals, std::vector<uint64_t> &lookingForValsAdd, std::vector<uint64_t> &lookingForValsXor, std::vector<uint64_t> &lookingForValsNegAdd, std::vector<uint64_t> &lookingForValsNotAdd, std::vector<uint64_t> &lookingForValsNegXor, std::vector<uint64_t> &lookingForValsNotXor) {
    lookingForValsAdd.reserve(newProgVals.size());
    lookingForValsXor.reserve(newProgVals.size());
    lookingForValsNegAdd.reserve(newProgVals.size());
    lookingForValsNotAdd.reserve(newProgVals.size());
    lookingForValsNegXor.reserve(newProgVals.size());
    lookingForValsNotXor.reserve(newProgVals.size());
    for(int i = 0; i < newProgVals.size(); i++) {
        lookingForValsAdd.push_back(expectedOutputsVals.at(i) - newProgVals.at(i));
        lookingForValsXor.push_back(expectedOutputsVals.at(i) ^ newProgVals.at(i));
        lookingForValsNegAdd.push_back((-expectedOutputsVals.at(i)) - newProgVals.at(i));
        lookingForValsNotAdd.push_back((~expectedOutputsVals.at(i)) - newProgVals.at(i));
        lookingForValsNegXor.push_back((-expectedOutputsVals.at(i)) ^ newProgVals.at(i));
        lookingForValsNotXor.push_back((~expectedOutputsVals.at(i)) ^ newProgVals.at(i));
    }
}

std::optional<ASTNode*> XorAddBottomUpDeduction::findMatchingProg(ASTNode*newProg, const std::vector<uint64_t> &newProgVals, const std::vector<uint64_t> &lookingForValsAdd, const std::vector<uint64_t> &lookingForValsXor, const std::vector<uint64_t> &lookingForValsNegAdd, const std::vector<uint64_t> &lookingForValsNotAdd, const std::vector<uint64_t> &lookingForValsNegXor, const std::vector<uint64_t> &lookingForValsNotXor) {
    if(progBucket.contains(lookingForValsAdd) && !noAddMaker) {
        ASTNode* addProg = addMaker->apply({newProg, progBucket.at(lookingForValsAdd)}, contexts);
        addProg->setMaker(addMaker);
        if(addProg->getValues() == expectedOutputs) {
            return addProg;
        }
        delete addProg;
    }
    if(progBucket.contains(lookingForValsXor) && !noXorMaker) {
        ASTNode* xorProg = xorMaker->apply({newProg, progBucket.at(lookingForValsXor)}, contexts);
        xorProg->setMaker(xorMaker);
        if(xorProg->getValues() == expectedOutputs) {
            return xorProg;
        }
        delete xorProg;
    }
    if(progBucket.contains(lookingForValsNegAdd) && !noNegMaker && !noAddMaker) {
        ASTNode* addProg = addMaker->apply({newProg, progBucket.at(lookingForValsNegAdd)}, contexts);
        addProg->setMaker(addMaker);
        ASTNode* negProg = negMaker->apply({addProg}, contexts);
        negProg->setMaker(negMaker);
        if(negProg->getValues() == expectedOutputs) {
            return negProg;
        }
        delete negProg;
    }
    if(progBucket.contains(lookingForValsNotAdd) && !noNotMaker && !noAddMaker) {
        ASTNode* addProg = addMaker->apply({newProg, progBucket.at(lookingForValsNotAdd)}, contexts);
        addProg->setMaker(addMaker);
        ASTNode* notProg = notMaker->apply({addProg}, contexts);
        notProg->setMaker(notMaker);
        if(notProg->getValues() == expectedOutputs) {
            return notProg;
        }
        delete notProg;
    }
    if(progBucket.contains(lookingForValsNegXor) && !noNegMaker && !noXorMaker) {
        ASTNode* xorProg = xorMaker->apply({newProg, progBucket.at(lookingForValsNegXor)}, contexts);
        xorProg->setMaker(xorMaker);
        ASTNode* negProg = negMaker->apply({xorProg}, contexts);
        negProg->setMaker(negMaker);
        if(negProg->getValues() == expectedOutputs) {
            return negProg;
        }
        delete negProg;
    }
    if(progBucket.contains(lookingForValsNotXor) && !noNotMaker && !noXorMaker) {
        ASTNode* xorProg = xorMaker->apply({newProg, progBucket.at(lookingForValsNotXor)}, contexts);
        xorProg->setMaker(xorMaker);
        ASTNode* notProg = notMaker->apply({xorProg}, contexts);
        notProg->setMaker(notMaker);
        if(notProg->getValues() == expectedOutputs) {
            return notProg;
        }
        delete notProg;
    }
    return std::nullopt;
}