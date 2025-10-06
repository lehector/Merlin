
#ifndef SIZEPORTFOLIOBV_H
#define SIZEPORTFOLIOBV_H
#include "SizeBasedPortfolio.h"


class SizePortfolioBV : public SizeBasedPortfolio {
public:
    SizePortfolioBV(const std::vector<std::map<std::string, variant>> &contexts,
        const std::shared_ptr<OEValuesManager> &oeManager, const std::vector<variant> &expectedOutputs,
        const std::shared_ptr<VocabFactory> &vocabFactory);
};



#endif //SIZEPORTFOLIOBV_H
