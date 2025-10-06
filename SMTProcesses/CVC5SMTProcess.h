
#ifndef MYSOLVER_CVC5SMTPROCESS_H
#define MYSOLVER_CVC5SMTPROCESS_H

#include "SMTProcess.h"
#include <cvc5/cvc5.h>
#include <cvc5/cvc5_parser.h>

class CVC5SMTProcess : public SMTProcess {
private:
    std::map<std::string, cvc5::Term> paramsCVC5Expressions;

    std::map<std::string, cvc5::Term> _getParamsCVC5Expressions(cvc5::parser::SymbolManager* sm);
    variant getValueFromExpression(const cvc5::Term& e, Helper::Type type, cvc5::Solver& slv);

    cvc5::parser::SymbolManager* parseStringIntoCVC(std::string smt, cvc5::parser::InputParser& parser, cvc5::Solver& slv);
    cvc5::Term getFunctionExpression(cvc5::parser::SymbolManager* sm);
    std::map<std::string, variant> getContextFromModel(cvc5::Solver& slv);
public:
    std::optional<std::unique_ptr<IOExample>> checkValidity(std::string programCode) override;

    explicit CVC5SMTProcess(SyGuSFileTask *syGuSFileTask);

    std::unique_ptr<IOExample> getExample(const std::vector<std::map<std::string, variant>> &currentContexts) override;
};


#endif //MYSOLVER_CVC5SMTPROCESS_H
