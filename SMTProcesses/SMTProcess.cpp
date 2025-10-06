
#include "SMTProcess.h"
#include "ranges"
#include "utility"
#include "../thirdparty/magic_enum/magic_enum.hpp"
#include <boost/log/trivial.hpp>

std::string SMTProcess::createSMTStringToReceiveExOutput(const std::map<std::string, variant>& context) {
    std::stringstream ss;
    ss << createSMTOutputPrefix;
    // constant evaluations
    for(const auto& [constName, constVal] : context) {
        ss << "(= " << constName << " " << Helper::variantToString(constVal) << ")\n";
    }
    ss << createSMTOutputPostfix;
    return ss.str();
}

std::string SMTProcess::createSMTStringToReceiveExOutputWithCurrentContexts(const std::vector<std::map<std::string, variant>>& contexts) {
    std::stringstream ss;
    ss << createSMTOutputPrefix;
    // constant evaluations
    for (const auto& context : contexts) {
        //ss << "(or ";
        for(const auto& [constName, constVal] : context) {
            ss << "(not (= " << constName << " " << Helper::variantToString(constVal) << "))\n";
        }
        //ss << " )";
    }
    ss << createSMTOutputPostfix;
    return ss.str();
}

std::pair<std::string, std::string> SMTProcess::createSMTStrings() {

    std::string lhs;
    std::string rhs;

    std::stringstream ss;
    ss << fileHead;

    std::vector<std::string> constraintStrings = syGuSFileTask->getConstraintSmtStrings();

    // define synth func
    ss << "(define-fun " << functionName << " ";
    ss << "(";
    for(const auto& [paramName, paramType] : functionParameters) {
        std::string paramTypeStr = Helper::typeToSmtString(paramType);
        ss << "(" << paramName << " " << paramTypeStr << ")";
    }
    ss << ")";
    std::string returnTypeString = Helper::typeToSmtString(returnType);
    ss << " " << returnTypeString << " ";

    lhs = ss.str();

    // ss << "FUNCDEF";
    ss.str(""); //  clear stream
    ss << ")\n";

    // declare const
    ss << functionParamConstDeclarations;



    ss << "(assert (not " << "\n";
    ss << " (and" << "\n";
    for(const std::string& constraintString : constraintStrings) {
        ss << constraintString << "\n";
    }
    ss << ") \n";
    ss << ") )" << "\n";

    ss << fileFoot;
    rhs = ss.str();
    return {lhs, rhs};
}

SMTProcess::SMTProcess(SyGuSFileTask *syGuSFileTask) : syGuSFileTask(syGuSFileTask){
    functionName = syGuSFileTask->getFunctionName();
    returnType = syGuSFileTask->getFunctionReturnType();
    std::vector<std::pair<std::string, Helper::Type>> paramsAsVector = syGuSFileTask->getFunctionParameters();
    for( const auto& [name, type] : paramsAsVector) {
        //functionParameters.emplace(name, type);
        functionParameters.emplace_back(name, type);
    }
    defineFuncStrings = syGuSFileTask->getDefineFuncSmtStrings();

    // setup File Head
    std::stringstream ss;
    ss << "(set-logic " << magic_enum::enum_name(syGuSFileTask->getLogic()) << ")" << "\n";
    ss << "(set-option :produce-models true)\n";
    for(const std::string& defineFuncString : defineFuncStrings) {
        ss << defineFuncString << "\n";
    }
    fileHead = ss.str();


    // setup file Foot
    ss.str(""); // Reset ss
    ss << "(check-sat) \n";
    ss << "(get-model) \n";
    fileFoot = ss.str();

    // declare function parameters as const vars
    ss.str(""); // reset ss
    for(const auto& [paramName, paramType] : functionParameters) {
        std::string paramTypeStr = Helper::typeToSmtString(paramType);
        ss << "(declare-const " << paramName << " " << paramTypeStr << ")\n";
    }
    functionParamConstDeclarations = ss.str();

    auto [outputPreFix,outputPostFix] = createSMTGetOutputPrePostFix();
    createSMTOutputPrefix = outputPreFix;
    createSMTOutputPostfix = outputPostFix;

    auto [inputPreFix,inputPostFix] = createSMTStrings();
    checkSMTPrefix = inputPreFix;
    checkSMTPostfix = inputPostFix;

}


std::pair<std::string, std::string> SMTProcess::createSMTGetOutputPrePostFix() {
    std::stringstream ss;
    ss << fileHead;


    std::vector<std::string> constraintStringsWithoutFuncCall = syGuSFileTask->getConstraintSmtStringsWithoutFuncCall();

    ss << functionParamConstDeclarations;
    // declare func const
    ss << "(declare-const " << functionName << " " << Helper::typeToSmtString(returnType) << ")\n";

    ss << "(assert " << "\n";
    ss << " (and" << "\n";
    for(const std::string& constraintString : constraintStringsWithoutFuncCall) {
        ss << constraintString << "\n";
    }

    std::string preFix = ss.str();

    //reset ss
    ss.str("");
    ss << ") \n";
    ss << ")" << "\n";

    ss << fileFoot;
    std::string postFix = ss.str();
    return {preFix, postFix};
}