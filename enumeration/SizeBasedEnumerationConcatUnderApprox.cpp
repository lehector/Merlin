
#include "SizeBasedEnumerationConcatUnderApprox.h"



std::optional<ASTNode *> SizeBasedEnumerationConcatUnderApprox::doBottomUpDeduction(ASTNode *newProg) {
    return concatBottomUpDeduction->doBottomUpDeduction(newProg);
    //return std::nullopt;
}

SizeBasedEnumerationConcatUnderApprox::SizeBasedEnumerationConcatUnderApprox(
    const std::shared_ptr<OEValuesManager> &oeManager, const std::shared_ptr<VocabFactory> &vocabFactory,
    const std::vector<std::map<std::string, variant>> &contexts, const std::vector<variant> &expectedOutputs,
    std::atomic<bool> *stop)
        : SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>(
            oeManager, vocabFactory, contexts, expectedOutputs, stop) {
        addSpecialLeafMakers();
        concatBottomUpDeduction = std::make_unique<ConcatBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory);
        radius = Config::GetInstance()->getConcatSubstrRadius();
    }

bool SizeBasedEnumerationConcatUnderApprox::getViolationStatic(const std::vector<variant> &progOutputs,
    const std::vector<variant> &expectedOutputs, Helper::Type progType, int radius) {
    if(progType != Helper::Type::String) return false;
    if (radius == 75) radius = 88;
    else if (radius == 50) radius = 75;
    else if (radius == 25) radius = 63;
    int lenDiff = 0;
    int targetLengths = 0;
    for(int i = 0; i < progOutputs.size(); i++) {
        const auto& valString = std::get<std::string>(progOutputs.at(i));
        //if(valString.size() > expectedOutputsVals.at(i).size()) return true;
        const auto& targetString = std::get<std::string>(expectedOutputs.at(i));
        if(!targetString.contains(valString)) return true;
        int targetLength = targetString.size();
        targetLengths += targetLength;
        lenDiff += targetLength - valString.size();
    }
    int newRadius = (targetLengths * radius + 99) / 100; // ceil of percentage
    return lenDiff > newRadius;
    //return lenDiff > expectedOutputs.size() * radius;
    //return false;
}

bool SizeBasedEnumerationConcatUnderApprox::getViolation(ASTNode *newProg) {
    return getViolationStatic(newProg->getValues(), expectedOutputs, newProg->getNodeType().getType(), radius);
}

std::vector<std::shared_ptr<VocabMaker>> SizeBasedEnumerationConcatUnderApprox::getSpecialLeafMakers() {
    return vocabFactory->getExtraLeaves(Helper::EnumerationMode::SizeBottomUpConcatUnderApprox);
    //return {};
}

void SizeBasedEnumerationConcatUnderApprox::resetEnumeration() {
    SizeBasedEnumeratorBottomUpDeductionUnderApprox<std::string>::resetEnumeration();
    concatBottomUpDeduction = std::make_unique<ConcatBottomUpDeduction>(this->contexts, this->expectedOutputsVals, this->expectedOutputs, vocabFactory);
}
