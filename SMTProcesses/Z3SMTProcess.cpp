
#include <boost/log/trivial.hpp>
#include "Z3SMTProcess.h"


variant Z3SMTProcess::getValueFromExpression(const z3::expr& e, const z3::model& m, Helper::Type type) {
    variant val;
    switch (type) {
        case Helper::Type::Int : {
            try{
                val = m.eval(e).get_numeral_int();
            }
            catch (const z3::exception& e){
                val = 0;
            }
            break;
        }
        case Helper::Type::String : {
            val = m.eval(e).get_string();
            break;
        }
        case Helper::Type::Bool : {
            val = m.eval(e).is_true();
            break;
        }
        case Helper::Type::BitVec64 : {
            val = m.eval(e).get_numeral_uint64();
            break;
        }
    }
    return val;
}


std::map<std::string, z3::expr> Z3SMTProcess::_getParamsZ3Expressions() {
    std::map<std::string, z3::expr> out;
    for(const auto& [paramName, paramType] : functionParameters) {
        z3::expr e = createExpression(paramName, paramType);
        out.emplace(paramName, e);
    }
    return out;
}

std::map<std::string, variant> Z3SMTProcess::getContextFromModel(const z3::model& m) {
    std::map<std::string, variant> context;
    for(const auto& [paramName, paramType] : functionParameters) {
        z3::expr paramExpr = paramsZ3Expressions.at(paramName);
        variant val = getValueFromExpression(paramExpr, m, paramType);

        context.emplace(paramName, val);
    }
    return context;
}

std::unique_ptr<IOExample> Z3SMTProcess::getExample(const std::vector<std::map<std::string, variant>> &currentContexts) {
    slv.push();
    std::string exampleOutSmtString = createSMTStringToReceiveExOutputWithCurrentContexts(currentContexts);
    auto e = ctx.parse_string(exampleOutSmtString.c_str());
    for(z3::expr exp : e) {
        slv.add(e);
    }
    z3::expr funcExpr = createExpression(functionName, returnType);
    if(slv.check() == z3::sat) {
        auto m = slv.get_model();
        auto context  = getContextFromModel(m);
        variant funcEval = getValueFromExpression(funcExpr, m, returnType);
        auto out =  std::make_unique<IOExample>(context, funcEval);
        return out;
    }
    else {
        BOOST_LOG_TRIVIAL(warning) << "UNREALIZABLE!";
    }
    slv.pop();
}

z3::expr Z3SMTProcess::createExpression(const std::string& paramName, Helper::Type paramType) {
    switch (paramType) {
        case Helper::Type::Int :{
            return ctx.int_const(paramName.c_str());
        }
        case Helper::Type::BitVec64 :{
            return ctx.bv_const(paramName.c_str(), 64);
        }
        case Helper::Type::String :{
            return ctx.string_const(paramName.c_str());
        }
        case Helper::Type::Bool :{
            return ctx.bool_const(paramName.c_str());
        }
    }
}

std::optional<std::unique_ptr<IOExample>> Z3SMTProcess::checkValidity(std::string programCode) {
    slv.push();
    std::optional<std::unique_ptr<IOExample>> out;
    std::string smtString = checkSMTPrefix + programCode + checkSMTPostfix;
    BOOST_LOG_TRIVIAL(debug) << "SMT String: \n" << smtString;
    z3::expr_vector e = ctx.parse_string(smtString.c_str()); // vllt kann man hier den prefix separat vorher parsen?? TODO
    for(z3::expr exp : e) {
        slv.add(e);
    }

    BOOST_LOG_TRIVIAL(trace) << "solving with z3";
    bool solved = slv.check() == z3::unsat;
    BOOST_LOG_TRIVIAL(trace) << "finish solving with z3";
    if(solved) return out;

    z3::model m = slv.get_model();
    std::map<std::string, variant> context = getContextFromModel(m);
    slv.pop();
    slv.push();
    std::string exampleOutSmtString = createSMTStringToReceiveExOutput(context);
    e = ctx.parse_string(exampleOutSmtString.c_str());
    for(z3::expr exp : e) {
        slv.add(e);
    }
    z3::expr funcExpr = createExpression(functionName, returnType);
    if(slv.check() == z3::sat) {
        m = slv.get_model();
        variant funcEval = getValueFromExpression(funcExpr, m, returnType);
        out =  std::make_unique<IOExample>(context, funcEval);
    }
    else {
        BOOST_LOG_TRIVIAL(warning) << "UNREALIZABLE!";
    }
    slv.pop();
    return out;
}

Z3SMTProcess::Z3SMTProcess(SyGuSFileTask *syGuSFileTask) : SMTProcess(syGuSFileTask) , slv(ctx) {
    paramsZ3Expressions = _getParamsZ3Expressions();
}
