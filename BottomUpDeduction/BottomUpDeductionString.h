
#ifndef BOTTOMUPDEDUCTIONSTRING_H
#define BOTTOMUPDEDUCTIONSTRING_H
#include "BottomUpDeduction.h"
#include <string>


class BottomUpDeductionString : public BottomUpDeduction {
protected:
    /**
     * output examples as strings
     */
    const std::vector<std::string>& expectedOutputsVals;

public:
    BottomUpDeductionString(const std::vector<std::map<std::string, variant>> &contexts,
        const std::vector<variant> &expected_outputs, const std::vector<std::string> &expected_outputs_vals)
        : BottomUpDeduction(contexts, expected_outputs),
          expectedOutputsVals(expected_outputs_vals) {
    }
};



#endif //BOTTOMUPDEDUCTIONSTRING_H
