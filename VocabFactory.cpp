
#include "VocabFactory.h"

#include <boost/log/trivial.hpp>

#include "thirdparty/magic_enum/magic_enum.hpp"

const std::vector<std::shared_ptr<VocabMaker>> &VocabFactory::getLeaves() const {
    return leaves;
}

const std::vector<std::shared_ptr<VocabMaker>> &VocabFactory::getNonLeaves() const {
    return nonLeaves;
}

VocabFactory::VocabFactory(const std::vector<std::shared_ptr<VocabMaker>> &makers) {
    for(const auto& maker : makers){
        if(maker->getArity() == 0) {
            leaves.push_back(maker);
        }
        else {
            nonLeaves.push_back(maker);
            if(maker->getHead() == "ite"){
                iteOperators.push_back(maker);
            }
            if(maker->getReturnType().getType() == Helper::Type::Bool){
                boolOperators.push_back(maker);
            }
        }
    }
}

const std::vector<std::shared_ptr<VocabMaker>> &VocabFactory::getIteOperators() const {
    return iteOperators;
}

void VocabFactory::addLeaveOperators(const std::vector<std::shared_ptr<VocabMaker>> &newLeaves, Helper::EnumerationMode forEnumerator) {
    for(const std::shared_ptr<VocabMaker> &leave: newLeaves) {
        BOOST_LOG_TRIVIAL(info) << "Added new maker: " << leave->getHead() << " for " << magic_enum::enum_name(forEnumerator);
    }
    extraLeaves[forEnumerator].insert(extraLeaves[forEnumerator].end(), newLeaves.begin(), newLeaves.end());
}

const std::vector<std::shared_ptr<VocabMaker>> & VocabFactory::getExtraLeaves(Helper::EnumerationMode forEnumerator) {
    return extraLeaves[forEnumerator];
}

void VocabFactory::moveIteOperatorsOutOfNonLeaves() {
    nonLeaves.erase(std::remove_if(nonLeaves.begin(),
                                    nonLeaves.end(),
                                    [](const std::shared_ptr<VocabMaker>& vocabMaker)-> bool
                                    { return vocabMaker->getHead() == "ite"; }),
                     nonLeaves.end());
}

void VocabFactory::moveBoolOperatorsOutOfNonLeaves() {
    nonLeaves.erase(std::remove_if(nonLeaves.begin(),
                                nonLeaves.end(),
                                [](const std::shared_ptr<VocabMaker>& vocabMaker)-> bool
                                { return vocabMaker->getReturnType().getType() == Helper::Type::Bool; }),
                 nonLeaves.end());
}
