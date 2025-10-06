
#include "SubstrBottomUpDeduction.h"

bool SubstrBottomUpDeduction::isLessThanEqPerVal(const std::vector<int>& a, const std::vector<int>& b) {
    for (int i = 0; i < a.size(); i++) {
        int lhs = a.at(i);
        int rhs = b.at(i);
        if (!(lhs <= rhs)) {
            return false;
        }
    }
    return true;
}

bool SubstrBottomUpDeduction::insertIntoLengthIntProgSet(ASTNode *prog, const std::vector<int>& vals) {
    if(!isLessThanEqPerVal(outputLengths, vals)) return false;
    auto ne = std::remove_if(lengthIntProgSet.begin(), lengthIntProgSet.end(), [vals](const std::pair<std::vector<int>, ASTNode*>& oldVals) {
        return SubstrBottomUpDeduction::isLessThanEqPerVal(vals, oldVals.first);
    });
    if(ne != lengthIntProgSet.end()) {
        lengthIntProgSet.erase(ne, lengthIntProgSet.end());
        lengthIntProgSet.emplace_back(vals, prog);
        return true;
    }
    bool thereIsBetter = false;
    for (const std::pair<std::vector<int>, ASTNode *> &progIntVecPair: lengthIntProgSet) {
        if (isLessThanEqPerVal(progIntVecPair.first, vals)) {
            thereIsBetter = true;
            break;
        }
    }
    if (!thereIsBetter) {
        lengthIntProgSet.emplace_back(vals, prog);
        return true;
    }
    return false;
}

std::optional<ASTNode *> SubstrBottomUpDeduction::traverseTooLongList(ASTNode *ast_node) {
    for(const auto& pair : tooLongList) {
        ASTNode *out = substrMaker->apply({pair.first, pair.second, ast_node}, contexts);
        out->setMaker(substrMaker);
        if (out->getValues() == expectedOutputs) {
            return out;
        }
        delete out;
    }
    return std::nullopt;
}

std::optional<ASTNode *> SubstrBottomUpDeduction::traverseLengthIntProgSet(ASTNode *stringProg, ASTNode *intProg) {
    for(const auto& [_, lengthProg] : lengthIntProgSet) {
        ASTNode *out = substrMaker->apply({stringProg, intProg, lengthProg}, contexts);
        out->setMaker(substrMaker);
        if (out->getValues() == expectedOutputs) {
            return out;
        }
        delete out;
    }
    return std::nullopt;
}

std::optional<ASTNode *> SubstrBottomUpDeduction::doBottomUpDeductionFromIntProg(ASTNode *newProg) {
    const std::vector<variant>& progVals = newProg->getValues();
    std::vector<int> vals;
    vals.reserve(progVals.size());
    for(const variant &progVal: progVals) {
        vals.push_back(std::get<int>(progVal));
    }
    bool isNewLengthProg = insertIntoLengthIntProgSet(newProg, vals);
    if(isNewLengthProg) {
        std::optional<ASTNode*> ret = traverseTooLongList(newProg);
        if(ret.has_value()) return ret;
    }
    intToProgMap.insert({vals, newProg});
    auto it = intToStringProgsMap.find(vals);
    if(it != intToStringProgsMap.end()) {
        for(ASTNode* candidate : it->second) {
            std::optional<ASTNode*> ret = traverseLengthIntProgSet(candidate, newProg);
            if(ret.has_value()) return ret;
        }
    }
    return std::nullopt;
}

std::optional<std::vector<int>> SubstrBottomUpDeduction::getSubstrStartPositions(const std::vector<variant>& progVals) {
    std::vector<int> substrStart;
    // get the starting positions of the expected outputs in the current output strings
    for(int i = 0; i < progVals.size(); i++) {
        auto valString = std::get<std::string>(progVals.at(i));
        int startPos = valString.find(expectedOutputsVals.at(i));
        if(startPos == std::string::npos) return std::nullopt; // not found
        substrStart.push_back(startPos);
    }
    return substrStart;
}

std::optional<ASTNode *> SubstrBottomUpDeduction::doBottomUpDeductionFromStringProg(ASTNode *newProg) {
    // not perfect, i am only considering first occurence, there might be better options
    const std::vector<variant>& progVals = newProg->getValues();
    std::optional<std::vector<int>> substrStartPositions = getSubstrStartPositions(progVals);
    if (!substrStartPositions.has_value()) return std::nullopt;
    std::vector<int> substrStart = substrStartPositions.value();

    intToStringProgsMap[substrStart].push_back(newProg);

    auto it = intToProgMap.find(substrStart);
    if(it != intToProgMap.end()) {
        ASTNode* intProg = it->second;
        for(const std::pair<std::vector<int>, ASTNode *> &lengthProgPair: lengthIntProgSet) {
            ASTNode* out = substrMaker->apply({newProg, intProg, lengthProgPair.second}, contexts);
            if(out->getValues() == expectedOutputs) {
                out->setMaker(substrMaker);
                return out;
            }
            delete out;
        }
        if(!lengthIntProgSet.empty()) {
            tooLongList.emplace_back(newProg, intProg);
        }
    }
    return std::nullopt;
}

SubstrBottomUpDeduction::SubstrBottomUpDeduction(const std::vector<std::map<std::string, variant>> &contexts,
                                                 const std::vector<variant> &expected_outputs, const std::vector<std::string> &expected_outputs_vals,
                                                 const std::shared_ptr<VocabFactory> &vocabFactory)
    : BottomUpDeductionString(contexts, expected_outputs, expected_outputs_vals) {
    for (const std::shared_ptr<VocabMaker> &maker: vocabFactory->getNonLeaves()) {
        if (maker->getHead() == "str.substr") {
            substrMaker = maker;
            noMaker = false;
        }
    }
    for(const std::string& expectedOutputVal : expectedOutputsVals) {
        outputLengths.push_back(expectedOutputVal.length());
    }
}

std::optional<ASTNode *> SubstrBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if (noMaker) return std::nullopt;
    if(newProg->getNodeType().getType() == Helper::Type::String) {
        return doBottomUpDeductionFromStringProg(newProg);
    }
    if(newProg->getNodeType().getType() == Helper::Type::Int) {
        return doBottomUpDeductionFromIntProg(newProg);
    }
    return std::nullopt;
}

void SubstrBottomUpDeduction::clear() {
    intToProgMap.clear();
    intToStringProgsMap.clear();
    lengthIntProgSet.clear();
    outputLengths.clear();
    tooLongList.clear();
    for(const std::string& expectedOutputVal : expectedOutputsVals) {
        outputLengths.push_back(expectedOutputVal.length());
    }
}