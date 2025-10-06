
#include "SizeBasedEnumerationSubstrUnderApprox.h"

#include <vector>
#include <vector>
#include <vector>
#include <vector>

std::optional<ASTNode *> SizeBasedEnumerationSubstrUnderApprox::doBottomUpDeduction(ASTNode *newProg) {
    return substrBottomUpDeduction->doBottomUpDeduction(newProg);
    return std::nullopt;
}

bool SizeBasedEnumerationSubstrUnderApprox::getViolation(ASTNode *newProg) {
    return getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumerationSubstrUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox);
}

SizeBasedEnumerationSubstrUnderApprox::SizeBasedEnumerationSubstrUnderApprox(
    const std::shared_ptr<OEValuesManager> &oeManager, const std::shared_ptr<VocabFactory> &vocabFactory,
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &expectedOutputs,
    std::atomic<bool> *stop)
    : SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>(
        oeManager, vocabFactory, contexts, expectedOutputs, stop) {
    addSpecialLeafMakers();
    substrBottomUpDeduction = std::make_unique<SubstrBottomUpDeduction>(this->contexts, this->expectedOutputs, this->expectedOutputsVals, vocabFactory);
    radius = Config::GetInstance()->getConcatSubstrRadius();
}

bool SizeBasedEnumerationSubstrUnderApprox::getViolationStatic(const std::vector<variant>& progOutputs,
    const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius) {
    if(progType == Helper::Type::String) {
        int lenDiff = 0;
        int targetLengths = 0;
        for(int i = 0; i < progOutputs.size(); i++) {
            const auto& valString = std::get<std::string>(progOutputs.at(i));
            //if(valString.size() < expectedOutputsVals.at(i).size()) return true;
            const auto& outString = std::get<std::string>(expectedOutputs.at(i));
            if(!valString.contains(outString)) return true;
            int targetLength = valString.size();
            lenDiff += targetLength - outString.size();
            targetLengths += 2 * targetLength; // i can be three times as large
        }
        if (radius == 100) return false; // 100 is everything that is a superstring
        int newRadius = (targetLengths * radius + 99) / 100; // ceil of percentage
        return lenDiff > newRadius;
        //return lenDiff > expectedOutputs.size() * newRadius;
    }
    return false;
}

void SizeBasedEnumerationSubstrUnderApprox::resetEnumeration() {
    SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>::resetEnumeration();
    substrBottomUpDeduction = std::make_unique<SubstrBottomUpDeduction>(this->contexts, this->expectedOutputs, this->expectedOutputsVals, vocabFactory);
}
