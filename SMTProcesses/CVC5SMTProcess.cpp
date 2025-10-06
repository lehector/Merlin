
#include <boost/log/trivial.hpp>
#include "boost/iostreams/stream.hpp"
#include "boost/iostreams/device/null.hpp"
#include "CVC5SMTProcess.h"



std::optional<std::unique_ptr<IOExample>> CVC5SMTProcess::checkValidity(std::string programCode) {
    cvc5::TermManager termManager1;
    cvc5::Solver slv1(termManager1);
    cvc5::parser::InputParser parser1(&slv1);
    std::optional<std::unique_ptr<IOExample>> out;

    std::string smtString = checkSMTPrefix + programCode + checkSMTPostfix;
    cvc5::parser::SymbolManager* sm = parseStringIntoCVC(smtString, parser1, slv1);

    // now, check sat with the solver
    BOOST_LOG_TRIVIAL(trace) << "solving with cvc5";
    cvc5::Result r = slv1.checkSat();
    bool solved = r.isUnsat();
    BOOST_LOG_TRIVIAL(trace) << "finish solving with cvc5";
    if(solved) return out;

    paramsCVC5Expressions = _getParamsCVC5Expressions(sm);

    std::map<std::string, variant> context = getContextFromModel(slv1);
    cvc5::TermManager termManager2;
    cvc5::Solver slv2(termManager1);
    cvc5::parser::InputParser parser2(&slv2);

    std::string exampleOutSmtString = createSMTStringToReceiveExOutput(context);
    sm = parseStringIntoCVC(exampleOutSmtString, parser2, slv2);
    cvc5::Term funcExpr = getFunctionExpression(sm);
    r = slv2.checkSat();
    if(r.isSat()) {
        variant funcEval = getValueFromExpression(funcExpr, returnType, slv2);
        out = std::make_unique<IOExample>(context, funcEval);
    }
    else {
        BOOST_LOG_TRIVIAL(warning) << "UNREALIZABLE!";
    }
    slv2.resetAssertions();

    return out;
}

CVC5SMTProcess::CVC5SMTProcess(SyGuSFileTask *syGuSFileTask) : SMTProcess(syGuSFileTask){
}

std::unique_ptr<IOExample> CVC5SMTProcess::getExample(const std::vector<std::map<std::string, variant>> &currentContexts) {
    cvc5::TermManager termManager1;
    cvc5::Solver slv1(termManager1);
    cvc5::parser::InputParser parser1(&slv1);
    std::optional<std::unique_ptr<IOExample>> out;

    std::string exampleOutSmtString = createSMTStringToReceiveExOutputWithCurrentContexts(currentContexts);
    cvc5::parser::SymbolManager* sm = parseStringIntoCVC(exampleOutSmtString, parser1, slv1);

    paramsCVC5Expressions = _getParamsCVC5Expressions(sm);

    cvc5::Result r = slv1.checkSat();
    if(r.isSat()) {
        auto context  = getContextFromModel(slv1);
        cvc5::Term funcExpr = getFunctionExpression(sm);
        variant funcEval = getValueFromExpression(funcExpr, returnType, slv1);
        auto out =  std::make_unique<IOExample>(context, funcEval);
        return out;
    }
    else {
        BOOST_LOG_TRIVIAL(warning) << "UNREALIZABLE!";
        return nullptr;
    }
}

std::map<std::string, cvc5::Term> CVC5SMTProcess::_getParamsCVC5Expressions(cvc5::parser::SymbolManager* sm) {
    std::map<std::string, cvc5::Term> out;
    for(const cvc5::Term& term : sm->getDeclaredTerms()) {
        std::string termName = term.getSymbol();
        if(termName == functionName) continue;
        //assert(functionParameters.contains(termName) && "out of thin air function parameter");
        out.emplace(termName, term);
    }
    return out;
}

variant CVC5SMTProcess::getValueFromExpression(const cvc5::Term &e, Helper::Type type, cvc5::Solver& slv) {
    variant val;
    switch (type) {
        case Helper::Type::Int : {
            val = slv.getValue(e).getInt32Value();
            break;
        }
        case Helper::Type::String : {
            std::wstring wide = slv.getValue(e).getStringValue();
            std::string out(wide.length(), 0);
            std::transform(wide.begin(), wide.end(), out.begin(), [] (wchar_t c) {
                return (char)c;
            });

            val = out;
            break;
        }
        case Helper::Type::Bool : {
            val = slv.getValue(e).getBooleanValue();
            break;
        }
        case Helper::Type::BitVec64 : {
            std::string out = slv.getValue(e).getBitVectorValue(16);
            val = strtoull(out.c_str(), nullptr, 16);
            break;
        }
    }
    return val;
}

std::map<std::string, variant> CVC5SMTProcess::getContextFromModel(cvc5::Solver& slv) {
    std::map<std::string, variant> context;
    for(const auto& [paramName, paramType] : functionParameters) {
        cvc5::Term paramExpr = paramsCVC5Expressions.at(paramName);
        variant val = getValueFromExpression(paramExpr, paramType, slv);

        context.emplace(paramName, val);
    }
    return context;
}

cvc5::parser::SymbolManager* CVC5SMTProcess::parseStringIntoCVC(std::string smt, cvc5::parser::InputParser& parser, cvc5::Solver& slv) {
    BOOST_LOG_TRIVIAL(debug) << "SMTString: \n" << smt;
    std::stringstream ss;
    ss << smt;
    parser.setStreamInput(cvc5::modes::InputLanguage::SMT_LIB_2_6, ss, "MyStream");
    // get the symbol manager of the parser, used when invoking commands below
    cvc5::parser::SymbolManager* sm = parser.getSymbolManager();
    // parse commands until finished
    cvc5::parser::Command cmd;
    boost::iostreams::stream< boost::iostreams::null_sink > nullOstream( ( boost::iostreams::null_sink() ) );
    while (true)
    {
        cmd = parser.nextCommand();

        if (cmd.isNull()) break;

        cmd.invoke(&slv, sm, nullOstream);
    }
    return sm;
}

cvc5::Term CVC5SMTProcess::getFunctionExpression(cvc5::parser::SymbolManager *sm) {
    for(const cvc5::Term& term : sm->getDeclaredTerms()) {
        std::string termName = term.getSymbol();
        if(termName == functionName) return term;
    }
    assert(false && "func not found");
}
