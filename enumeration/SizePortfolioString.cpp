
#include "SizePortfolioString.h"

#include "SizeBasedCombinedBottomUpStrings.h"
#include "SizeBasedEnumerationConcatUnderApprox.h"
#include "SizeBasedEnumerationReplaceUnderApprox.h"
#include "SizeBasedEnumerationSubstrUnderApprox.h"

SizePortfolioString::SizePortfolioString(const std::vector<std::map<std::string, variant> > &contexts,
                                         const std::shared_ptr<OEValuesManager> &oeManager,
                                         const std::vector<variant> &expectedOutputs,
                                         const std::shared_ptr<VocabFactory> &vocabFactory)
    : SizeBasedPortfolio(contexts, oeManager, expectedOutputs, vocabFactory) {
    if(Helper::typeOf(expectedOutputs.at(0)) == Helper::Type::String) {
        auto oeManager1 = std::make_shared<OEValuesManager>();
        enumerators.push_back(
            std::make_shared<SizeBasedEnumerationConcatUnderApprox>(oeManager1, vocabFactory, contexts, expectedOutputs,
                                                                    &stop));
        auto oeManager2 = std::make_shared<OEValuesManager>();
        enumerators.push_back(
            std::make_shared<SizeBasedEnumerationSubstrUnderApprox>(oeManager2, vocabFactory, contexts, expectedOutputs,
                                                                    &stop));
        auto oeManager3 = std::make_shared<OEValuesManager>();
        enumerators.push_back(
            std::make_shared<SizeBasedEnumerationReplaceUnderApprox>(oeManager3, vocabFactory, contexts, expectedOutputs,
                                                                    &stop));
    }
    auto oeManager4 = std::make_shared<OEValuesManager>();
    enumerators.push_back(std::make_shared<SizeBasedCombinedBottomUpStrings>(oeManager4, vocabFactory, contexts, expectedOutputs,
                                                                    &stop));
}
