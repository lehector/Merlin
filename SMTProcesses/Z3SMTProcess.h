
#ifndef MYSOLVER_Z3SMTPROCESS_H
#define MYSOLVER_Z3SMTPROCESS_H


#include "SMTProcess.h"
#include <z3++.h>

class Z3SMTProcess : public SMTProcess {
private:
    z3::context ctx;
    z3::solver slv;

    std::map<std::string, z3::expr> paramsZ3Expressions;

public:
    std::optional<std::unique_ptr<IOExample>> checkValidity(std::string programCode) override;

    explicit Z3SMTProcess(SyGuSFileTask *syGuSFileTask);

private:
    std::map<std::string, z3::expr> _getParamsZ3Expressions();
    z3::expr createExpression(const std::string& name, Helper::Type type);
    static variant getValueFromExpression(const z3::expr& e, const z3::model& m, Helper::Type type);

    std::map<std::string, variant> getContextFromModel(const z3::model& m);

public:
    std::unique_ptr<IOExample> getExample(const std::vector<std::map<std::string, variant>> &currentContexts) override;
};


#endif //MYSOLVER_Z3SMTPROCESS_H
