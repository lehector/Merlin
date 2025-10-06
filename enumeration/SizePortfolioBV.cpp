
#include "SizePortfolioBV.h"
#include "SizeBasedCombinedBottomUp.h"
#include "SizeBasedEnumerationXorAddHDUnderApprox.h"
#include "SizeBasedEnumeratorAndUnderApprox.h"
#include "SizeBasedEnumeratorMulUnderApprox.h"
#include "SizeBasedEnumeratorOrUnderApprox.h"

SizePortfolioBV::SizePortfolioBV(const std::vector<std::map<std::string, variant>> &contexts,
    const std::shared_ptr<OEValuesManager> &oeManager, const std::vector<variant> &expectedOutputs,
    const std::shared_ptr<VocabFactory> &vocabFactory)
        : SizeBasedPortfolio(contexts, oeManager, expectedOutputs, vocabFactory) {
    auto oeManager1 = std::make_shared<OEValuesManager>();
    enumerators.push_back(std::make_shared<SizeBasedCombinedBottomUp>(oeManager1, vocabFactory, contexts, expectedOutputs, &stop));
    auto oeManager2 = std::make_shared<OEValuesManager>();
    enumerators.push_back(std::make_shared<SizeBasedEnumeratorAndUnderApprox>(oeManager2, vocabFactory, contexts, expectedOutputs, &stop));
    auto oeManager3 = std::make_shared<OEValuesManager>();
    enumerators.push_back(std::make_shared<SizeBasedEnumeratorOrUnderApprox>(oeManager3, vocabFactory, contexts, expectedOutputs, &stop));
    auto oeManager4 = std::make_shared<OEValuesManager>();
    enumerators.push_back(std::make_shared<SizeBasedEnumeratorMulUnderApprox>(oeManager4, vocabFactory, contexts, expectedOutputs, &stop));
    auto oeManager5 = std::make_shared<OEValuesManager>();
    enumerators.push_back(std::make_shared<SizeBasedEnumerationXorAddHDUnderApprox>(oeManager5, vocabFactory, contexts, expectedOutputs, &stop));
}
