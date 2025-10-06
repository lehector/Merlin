
#include "SizeBasedEnumeratorHDUnderApprox.h"


std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumeratorHDUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpHDUnderApprox);
}

std::optional<ASTNode*> SizeBasedEnumeratorHDUnderApprox::doBottomUpDeduction(
    ASTNode*newProg) {
    return std::nullopt;
}

bool SizeBasedEnumeratorHDUnderApprox::isNotImprovement(ASTNode *newProg, const std::vector<variant> &outputs, int progHd) {
    for(auto child : newProg->getChildren()) {
        if(child->getTerms() < Config::GetInstance()->getCutoffSize()) continue;
        bool childIsViolation = false;
        int childHd = 0;
        for(int i = 0; i < outputs.size(); i++) {
            const auto &val = std::get<uint64_t>(child->getValues().at(i));
            const auto &target = std::get<uint64_t>(outputs.at(i));
            int childHd_ = Helper::hammingDistance(val, target);
            //if (childHd_ > hdWindowLowerBarrier && childHd_ < hdWindowUpperBarrier) {
            //    childIsViolation = true;
            //    break;
            //}
            childHd_ = childHd_ < 32 ? childHd_ : 64 - childHd_;
            childHd += childHd_;
        }
        if(!childIsViolation && progHd > childHd) {
            return true;
        }
    }
    return false;
}

SizeBasedEnumeratorHDUnderApprox::SizeBasedEnumeratorHDUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
    const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
    const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop)
        : SizeBasedEnumeratorBottomUpDeductionUnderApprox(oeManager, vocabFactory, contexts, expectedOutputs, stop) {
        radius = Config::GetInstance()->getHdRadius();
    }

bool SizeBasedEnumeratorHDUnderApprox::getViolationStatic(const std::vector<variant> &progOutputs,
                                                          const std::vector<variant> &outputs, Helper::Type nodeType, int radius) {
    int bvDists = 0;
    for(int i = 0; i < outputs.size(); i++) {
        const auto& val = progOutputs.at(i);
        const auto& target = outputs.at(i);
        int hd = Helper::hammingDistance(std::get<uint64_t>(val), std::get<uint64_t>(target));
        int bvDist = hd <= 32 ? hd : 64 - hd + 1;
        bvDists += bvDist;
    }
    int maxBvDistSum = 32 * (int) outputs.size();
    int newRadius = (maxBvDistSum * radius + 99) / 100; // ceil of percentage
    int ret = bvDists > newRadius;
    return ret;
}

bool SizeBasedEnumeratorHDUnderApprox::getViolation(ASTNode*newProg) {
    return getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}
