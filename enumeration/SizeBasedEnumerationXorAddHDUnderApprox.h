
#ifndef SIZEBASEDENUMERATIONXORADDHDUNDERAPPROX_H
#define SIZEBASEDENUMERATIONXORADDHDUNDERAPPROX_H
#include "SizeBasedEnumeratorXorAdd.h"


class SizeBasedEnumerationXorAddHDUnderApprox : public SizeBasedEnumeratorXorAdd {
    /**
     * radius percentage
     */
    int radius;
public:
    SizeBasedEnumerationXorAddHDUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs, std::atomic<bool> *stop)
        : SizeBasedEnumeratorXorAdd(oeManager, vocabFactory, contexts, expectedOutputs, stop), radius(Config::GetInstance()->getHdRadius()) {
    }

    SizeBasedEnumerationXorAddHDUnderApprox(const std::shared_ptr<OEValuesManager> &oeManager,
        const std::shared_ptr<VocabFactory> &vocabFactory, const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expectedOutputs)
        : SizeBasedEnumeratorXorAdd(oeManager, vocabFactory, contexts, expectedOutputs), radius(Config::GetInstance()->getHdRadius()){
    }

protected:
    bool getViolation(ASTNode *newProg) override;
};



#endif //SIZEBASEDENUMERATIONXORADDHDUNDERAPPROX_H
