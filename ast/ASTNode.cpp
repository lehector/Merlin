#include <iomanip>
#include "ASTNode.h"

#include "../Config.h"
#include "../CustomVocabMaker.h"
#include "../VocabMaker.h"
#include "../enumeration/SizeBasedEnumerationSubstrUnderApprox.h"
#include "../enumeration/SizeBasedEnumerationConcatUnderApprox.h"
#include "../enumeration/SizeBasedEnumeratorAndUnderApprox.h"
#include "../enumeration/SizeBasedEnumeratorHDUnderApprox.h"
#include "../enumeration/SizeBasedEnumeratorOrUnderApprox.h"
#include "../enumeration/SizeBasedEnumeratorMulUnderApprox.h"

NonTerminal ASTNode::getNodeType() const {
    return nodeType;
}

std::string ASTNode::getCode() const {
    if(children.empty()) return head;
    std::stringstream ss;
    ss << "(" << head << " ";
    for(int i = 0; i< children.size(); i++) {
        ss << children.at(i)->getCode();
        if(i != children.size() - 1) {
            ss << " ";
        }
    }
    ss << ")";
    std::string out = ss.str();
    return out;
}

const std::string & ASTNode::getHead() const {
    return head;
}

int ASTNode::getHeight() const {
    return height;
}

int ASTNode::getTerms() const {
    return terms;
}

int ASTNode::getRealSize() const {
    int size = 1;
    for(ASTNode *const&child: children) {
        size += child->getRealSize();
    }
    return size;
}

const std::vector<ASTNode*> &ASTNode::getChildren() const {
    return children;
}

int ASTNode::getCost() const {
    return cost;
}

const std::vector<variant> &ASTNode::getValues() const {
    return values;
}

std::pair<std::vector<uint64_t>, std::vector<uint64_t>> ASTNode::extractValuesAndTargets(
    const std::vector<std::map<std::string, variant> > &contexts, const std::vector<variant> &targets) const {
    std::vector<variant> vals = eval(contexts);
    std::vector<uint64_t> bitvectorVals;
    for(const variant &val: vals) {
        bitvectorVals.push_back(std::get<uint64_t>(val));
    }
    std::vector<uint64_t> bitvectorTargets;
    for(const variant& target : targets) {
        bitvectorTargets.push_back(std::get<uint64_t>(target));
    }
    assert(bitvectorVals.size() == bitvectorTargets.size());
    return {bitvectorVals, bitvectorTargets};
}

std::vector<std::shared_ptr<VocabMaker>> ASTNode::getAndMaximalComponentsMakers(const std::vector<std::map<std::string, variant>> &contexts,
                                                       const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::BitVec64) return {};
    std::vector<std::shared_ptr<VocabMaker>> ret;
    //auto[bitvectorVals, bitvectorTargets] = extractValuesAndTargets(contexts, targets);
    int radius = Config::GetInstance()->getAndOrRadius();
    bool violation = SizeBasedEnumeratorAndUnderApprox::getViolationStatic(eval(contexts), targets, nodeType.getType(), radius);
    if(!violation) {
        ret.push_back(generateVocabMaker(contexts));
    }
    for (ASTNode *child: children) {
        if(child->getTerms() <= 2) continue;
        std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getAndMaximalComponentsMakers(
            contexts, targets);
        ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
    }
    return ret;
}


std::vector<std::shared_ptr<VocabMaker>> ASTNode::getOrMaximalComponentsMakers(const std::vector<std::map<std::string, variant>> &contexts,
                                                       const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::BitVec64) return {};
    std::vector<std::shared_ptr<VocabMaker>> ret;
    int radius = Config::GetInstance()->getAndOrRadius();
    //auto[bitvectorVals, bitvectorTargets] = extractValuesAndTargets(contexts, targets);
    bool violation = SizeBasedEnumeratorOrUnderApprox::getViolationStatic(eval(contexts), targets, nodeType.getType(), radius);
    if(!violation) {
        ret.push_back(generateVocabMaker(contexts));
    }
    for (ASTNode *child: children) {
        if(child->getTerms() <= 2) continue;
        std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getOrMaximalComponentsMakers(
            contexts, targets);
        ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
    }
    return ret;
}

std::vector<std::shared_ptr<VocabMaker>> ASTNode::getUseFulFactors(const std::vector<std::map<std::string, variant>> &contexts,
                                                       const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::BitVec64) return {};
    std::vector<std::shared_ptr<VocabMaker>> ret;
    //auto[bitvectorVals, bitvectorTargets] = extractValuesAndTargets(contexts, targets);
    int radius = Config::GetInstance()->getMulRadius();
    bool violation = SizeBasedEnumeratorMulUnderApprox::getViolationStatic(eval(contexts), targets, nodeType.getType(), radius);
    if(getRealSize() > 12) {
        violation = true;
    }
    if(!violation) {
        ret.push_back(generateVocabMaker(contexts));
    }
    for (ASTNode *child: children) {
        if(child->getTerms() <= 2) continue;
        std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getUseFulFactors(
            contexts, targets);
        ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
    }
    return ret;
}

std::vector<std::shared_ptr<VocabMaker>> ASTNode::getCloseProgs(const std::vector<std::map<std::string, variant>> &contexts,
                                                       const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::BitVec64) return {};
    std::vector<std::shared_ptr<VocabMaker>> ret;
    //auto[bitvectorVals, bitvectorTargets] = extractValuesAndTargets(contexts, targets);
    int radius = Config::GetInstance()->getHdRadius();
    bool violation = SizeBasedEnumeratorHDUnderApprox::getViolationStatic(eval(contexts), targets, nodeType.getType(), radius);
    if(!violation) {
        ret.push_back(generateVocabMaker(contexts));
    }
    for (ASTNode *child: children) {
        if(child->getTerms() <= 2) continue;
        std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getCloseProgs(
            contexts, targets);
        ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
    }
    return ret;
}

std::vector<std::shared_ptr<VocabMaker>> ASTNode::getUsefulReplacements(
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::String) return {};
    std::vector<std::shared_ptr<VocabMaker>> ret;
    if(head == "str.replace") {
        std::vector<variant> thingsToBeReplaced = children.at(1)->eval(contexts);
        bool violation = false;
        for(int i = 0; i < contexts.size(); i++) {
            std::string str = std::get<std::string>(thingsToBeReplaced.at(i));
            if(std::get<std::string>(targets.at(i)).contains(str)) {
                violation = true;
                break;
            }
        }
        if (!violation) {
            ret.push_back(generateVocabMaker(contexts));
            // thing that is replaced might be correct, but the whole replacement might not
            if (children.at(1)->getTerms() > 1) {
                ret.push_back(children.at(1)->generateVocabMaker(contexts));
            }
        }
        for (ASTNode *child: children) {
            if (child->getTerms() <= 2) continue;
            std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getUsefulReplacements(
                contexts, targets);
            ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
        }
    }
    return ret;
}

std::vector<std::shared_ptr<VocabMaker>> ASTNode::getUsefulSubstrings(
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::String) return {};
    std::vector<std::shared_ptr<VocabMaker> > ret;
    std::vector<variant> vals = eval(contexts);
    int radius = Config::GetInstance()->getConcatSubstrRadius();
    bool violation = SizeBasedEnumerationConcatUnderApprox::getViolationStatic(eval(contexts), targets, nodeType.getType(), radius);
    if (!violation) {
        ret.push_back(generateVocabMaker(contexts));
    }
    for (ASTNode *child: children) {
        if (child->getTerms() <= 2) continue;
        std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getUsefulSubstrings(
            contexts, targets);
        ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
    }
    return ret;
}

std::vector<std::shared_ptr<VocabMaker>> ASTNode::getUsefulSuperstrings(
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &targets) const {
    if(nodeType.getType() != Helper::String) return {};
    std::vector<std::shared_ptr<VocabMaker> > ret;
    std::vector<variant> vals = eval(contexts);
    int radius = Config::GetInstance()->getConcatSubstrRadius();
    bool violation = SizeBasedEnumerationSubstrUnderApprox::getViolationStatic(eval(contexts), targets, nodeType.getType(), radius);
    if (!violation) {
        ret.push_back(generateVocabMaker(contexts));
    }
    for (ASTNode *child: children) {
        if (child->getTerms() <= 2) continue;
        std::vector<std::shared_ptr<VocabMaker> > maxChildrenMakers = child->getUsefulSuperstrings(
            contexts, targets);
        ret.insert(ret.end(), maxChildrenMakers.begin(), maxChildrenMakers.end());
    }
    return ret;
}


std::shared_ptr<VocabMaker> ASTNode::generateVocabMaker(const std::vector<std::map<std::string, variant>> &contexts) const {
    //auto maker = new CustomVocabMaker(0, {}, nodeType, getCode(), deepCopy(contexts), contexts);
    return std::make_shared<CustomVocabMaker>(nodeType, getCode(), deepCopy(contexts), contexts);
    //return nullptr;
}

ASTNode::~ASTNode() {
    if(recursivelyDelete) {
        for(ASTNode *child: children) {
            delete child;
        }
    }
}

uint64_t innerProduct(uint64_t a, uint64_t b){
    uint64_t innerProuct = a & b;
    return Helper::countOnes(innerProuct);
}

void ASTNode::setTerms(int terms) {
    this->terms = terms;
}

std::string ASTNode::getTrace(const std::vector<variant>& targets) {
    Config* config = Config::GetInstance();
    //if(this->terms <= config->getCutoffSize() - 1) return "";
    std::stringstream stringstream;
    stringstream << "Prog: " << std::setw(80) << this->getCode() << "; ";
    //stringstream << "Value: " << Helper::variantToString(this->values) << "; ";
    if(nodeType.getType() == Helper::BitVec64) {
        stringstream << "Cum. Hamming Distance: ";
        int hd = 0;
        for(int i = 0; i < targets.size(); i++) {
            uint64_t val = std::get<uint64_t>(values.at(i));
            uint64_t target = std::get<uint64_t>(targets.at(i));
            hd += Helper::hammingDistance(val, target);
            //stringstream << std::setw(2) << hd;
            //if(i != targets.size() - 1) {
            //    stringstream << ", ";
            //}
        }
        stringstream << std::setw(3) << hd;

        //stringstream << "; Num Trailing Zeros: ";
        //for(int i = 0; i < targets.size(); i++) {
        //    uint64_t val = std::get<uint64_t>(values.at(i));
        //    int valTrailingZeros = Helper::countTrailingZeros(val);
        //    stringstream << std::setw(2) << valTrailingZeros;
        //    if(i != targets.size() - 1) {
        //        stringstream << ", ";
        //    }
        //}

        stringstream << "; And Distance: ";
        //if(SizeBasedEnumeratorAndUnderApprox::getViolationStatic(this, targets, false)) {
        //    stringstream << "violation";
        //}
        //else {
        //    uint myscore = 0;
        //    for(int i = 0; i < targets.size(); i++) {
        //        uint64_t val = std::get<uint64_t>(values.at(i));
        //        uint64_t target = std::get<uint64_t>(targets.at(i));
        //        myscore += Helper::hammingDistance(val, target);
        //    }
        //    stringstream << std::to_string(myscore);
        //}

        if(!children.empty()) {
            stringstream << "; Relative Unsigned Distance improved: ";
            for (int i = 0; i < targets.size(); i++) {
                uint64_t val = std::get<uint64_t>(values.at(i));
                uint64_t valTarget = std::get<uint64_t>(targets.at(i));
                uint64_t absDistance = valTarget - val;
                if(val > valTarget) absDistance = val - valTarget;
                bool improved = true;
                for(const auto & child : children) {
                    uint64_t valChild = std::get<uint64_t>(child->getValues().at(i));
                    uint64_t absDistanceChild = valTarget - valChild;
                    if (valChild > valTarget) absDistanceChild = valChild - valTarget;
                    if(absDistance > absDistanceChild) {
                        improved = false;
                        break;
                    }
                }
                stringstream << improved;
                if (i != targets.size() - 1) {
                    stringstream << ", ";
                }
            }
        }

        if(!children.empty()) {
            stringstream << "; Relative Signed Distance improved: ";
            for (int i = 0; i < targets.size(); i++) {
                auto val = (int64_t) std::get<uint64_t>(values.at(i));
                auto valTarget = (int64_t) std::get<uint64_t>(targets.at(i));
                auto absDistance = valTarget - val;
                if(val > valTarget) absDistance = val - valTarget;
                bool improved = true;
                for(const auto & child : children) {
                    auto valChild = (int64_t) std::get<uint64_t>(child->getValues().at(i));
                    auto absDistanceChild = valTarget - valChild;
                    if (valChild > valTarget) absDistanceChild = valChild - valTarget;
                    if(absDistance > absDistanceChild) {
                        improved = false;
                        break;
                    }
                }
                stringstream << improved;
                if (i != targets.size() - 1) {
                    stringstream << ", ";
                }
            }
        }

        //stringstream << "; Is <= unsigned: ";
        //for(int i = 0; i < targets.size(); i++) {
        //    uint64_t val = std::get<uint64_t>(values.at(i));
        //    bool lessThanUnsigned = val <= std::get<uint64_t>(targets.at(i));
        //    stringstream << lessThanUnsigned;
        //    if(i != targets.size() - 1) {
        //        stringstream << ", ";
        //    }
        //}

        //stringstream << "; Is <= signed: ";
        //for(int i = 0; i < targets.size(); i++) {
        //    auto val = (int64_t) std::get<uint64_t>(values.at(i));
        //    bool lessThanSigned = val <= ((int64_t) std::get<uint64_t>(targets.at(i)));
        //    stringstream << lessThanSigned;
        //    if(i != targets.size() - 1) {
        //        stringstream << ", ";
        //    }
        //}
        //stringstream << "; Is <= absolute: ";
        //for(int i = 0; i < targets.size(); i++) {
        //    auto val = (int64_t) std::get<uint64_t>(values.at(i));
        //    if(val < 0) val = -val;
        //    auto targetVal = (int64_t) std::get<uint64_t>(targets.at(i));
        //    if(targetVal < 0) targetVal = -targetVal;
        //    bool lessThanSigned = val <= targetVal;
        //    stringstream << lessThanSigned;
        //    if(i != targets.size() - 1) {
        //        stringstream << ", ";
        //    }
        //}
        //stringstream << "; Absolute Distance: ";
        //for(int i = 0; i < targets.size(); i++) {
        //    auto val = (uint64_t) std::get<uint64_t>(values.at(i));
        //    if(val < 0) val = -val;
        //    auto targetVal = (uint64_t) std::get<uint64_t>(targets.at(i));
        //    if(targetVal < 0) targetVal = -targetVal;
        //    uint64_t absoluteDistance = targetVal - val;
        //    if(val >= targetVal) absoluteDistance = val - targetVal;
        //    stringstream << std::setw(10) << absoluteDistance;
        //    if(i != targets.size() - 1) {
        //        stringstream << ", ";
        //    }
        //}
    }
    stringstream << "\n";
    for(ASTNode*child: children) {
        stringstream << child->getTrace(targets);
    }
    return stringstream.str();
}

void ASTNode::setRecursivelyDelete(bool recursively_delete) {
    recursivelyDelete = recursively_delete;
}

std::weak_ptr<VocabMaker> ASTNode::getMaker() const {
    return maker;
}

void ASTNode::setMaker(const std::shared_ptr<VocabMaker> &maker) {
    this->maker = maker;
    makerIsSet = true;
}

ASTNode * ASTNode::deepCopy(const std::vector<std::map<std::string, variant>>& contexts) const {
    std::vector<ASTNode*> childrenCopies;
    childrenCopies.reserve(children.size());
    for(ASTNode *const&child: children) {
        childrenCopies.push_back(child->deepCopy(contexts));
    }
    std::shared_ptr<VocabMaker> makerShrdPointer = maker.lock();
    ASTNode* myCopy = makerShrdPointer->apply(childrenCopies, contexts);
    myCopy->setRecursivelyDelete(true);
    myCopy->setMaker(makerShrdPointer);
    return myCopy;
}
