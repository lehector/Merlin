
#ifndef BOTTOMUPDEDUCTIONBV_H
#define BOTTOMUPDEDUCTIONBV_H
#include "BottomUpDeduction.h"


class BottomUpDeductionBV : public BottomUpDeduction {
protected:
    /**
     * output examples as bitvecs
     */
    const std::vector<uint64_t>& expectedOutputsVals;

public:
    BottomUpDeductionBV(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expected_outputs, const std::vector<uint64_t> &expected_outputs_vals)
        : BottomUpDeduction(contexts, expected_outputs),
          expectedOutputsVals(expected_outputs_vals) {
    }
};



#endif //BOTTOMUPDEDUCTIONBV_H
