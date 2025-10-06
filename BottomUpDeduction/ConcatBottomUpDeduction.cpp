
#include "ConcatBottomUpDeduction.h"

std::optional<ASTNode*> ConcatBottomUpDeduction::traverseBucket(const std::vector<std::pair<std::string, std::string>> &valTargetPairs,
    ASTNode* newProg,
    bool start,
    const boost::unordered::detail::iterator_detail::iterator<boost::unordered::detail::node<std::pair<const std::string
    , std::vector<ASTNode *>>, void *>, boost::unordered::detail::bucket<boost::unordered::detail::node<std::pair<const
    std::string, std::vector<ASTNode *>>, void *>, void *>> &it) {
    for (ASTNode *candidate: it->second) {
        bool violation = false;
        for (int i = 0; i < valTargetPairs.size(); i++) {
            std::string contextCandidate = std::get<std::string>(candidate->getValues().at(candidate->getValues().size() - 1 - i));
            if(start) {
                if ((valTargetPairs.at(i).first + contextCandidate) != valTargetPairs.at(i).second) {
                    violation = true;
                    break;
                };
            }
            else {
                 if ((contextCandidate + valTargetPairs.at(i).first) != valTargetPairs.at(i).second) {
                    violation = true;
                    break;
                };
            }
        }
        if (violation) continue;
        //BOOST_LOG_TRIVIAL(info) << "found prog that works on last " << valTargetPairs.size() << " values with and";
        ASTNode* concatProg;
        if(start) {
            concatProg = concatMaker->apply({newProg, candidate}, contexts);
        }
        else {
            concatProg = concatMaker->apply({candidate, newProg}, contexts);
        }
        concatProg->setMaker(concatMaker);
        if (concatProg->getValues() == expectedOutputs) {
            return concatProg;
        }
        delete concatProg;
    }
    return std::nullopt;
}

std::optional<ASTNode *> ConcatBottomUpDeduction::doBottomUpDeduction(ASTNode *newProg) {
    if (noMaker) return std::nullopt;
    if(newProg->getNodeType().getType() != Helper::Type::String) return std::nullopt;
    const std::vector<variant> &progValues = newProg->getValues();
    const auto& firstVal = std::get<std::string>(progValues.at(0));
    firstContextToProgsMap[firstVal].push_back(newProg);
    const std::string &firstTarget = expectedOutputsVals.at(0);
    int numContexts = progValues.size();
    int min = std::min(numContexts, config->getFirstNValuesCheck());
    std::vector<std::pair<std::string, std::string> > valTargetPairs;
    valTargetPairs.reserve(min);
    for (int i = 0; i < min; i++) {
        valTargetPairs.emplace_back(std::get<std::string>(progValues.at(progValues.size() - 1 - i)),
                                    expectedOutputsVals.at(expectedOutputsVals.size() - 1 - i));
    }
    if (firstTarget.starts_with(firstVal)) {
        std::string end = firstTarget.substr(firstVal.size());
        if (auto it = firstContextToProgsMap.find(end); it != firstContextToProgsMap.end()) {
            auto ret = traverseBucket(valTargetPairs,  newProg, true, it);
            if(ret.has_value()) return ret;
        }
    }
    if(firstTarget.ends_with(firstVal)) {
        std::string start = firstTarget.substr(0, firstTarget.size() - firstVal.size());
        if (auto it = firstContextToProgsMap.find(start); it != firstContextToProgsMap.end()) {
            return traverseBucket(valTargetPairs, newProg, false, it);
        }
    }
    return std::nullopt;
}

void ConcatBottomUpDeduction::clear() {
    firstContextToProgsMap.clear();
}

ConcatBottomUpDeduction::ConcatBottomUpDeduction(const std::vector<std::map<std::string, variant> > &contexts,
                                                 const std::vector<std::string> &expected_outputs_vals,
                                                 const std::vector<variant> &expected_outputs,
                                                 const std::shared_ptr<VocabFactory> &vocabFactory) : BottomUpDeductionString(
    contexts, expected_outputs, expected_outputs_vals) {
    config = Config::GetInstance();
    for (const std::shared_ptr<VocabMaker> &maker: vocabFactory->getNonLeaves()) {
        if (maker->getHead() == "str.++") {
            concatMaker = maker;
            noMaker = false;
        }
    }
}
