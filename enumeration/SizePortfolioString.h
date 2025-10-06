
#ifndef SIZEPORTFOLIOSTRING_H
#define SIZEPORTFOLIOSTRING_H
#include "SizeBasedPortfolio.h"


class SizePortfolioString : public SizeBasedPortfolio {
public:
    SizePortfolioString(const std::vector<std::map<std::string, variant>> &contexts,
        const std::shared_ptr<OEValuesManager> &oeManager, const std::vector<variant> &expectedOutputs,
        const std::shared_ptr<VocabFactory> &vocabFactory);
};



#endif //SIZEPORTFOLIOSTRING_H
