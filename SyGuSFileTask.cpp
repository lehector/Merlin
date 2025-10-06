
#include "SyGuSFileTask.h"
#include <boost/log/trivial.hpp>
#include <boost/algorithm/string.hpp>

#include <utility>
#include "SygusLexer.h"
#include "thirdparty/magic_enum/magic_enum.hpp"
#include "ast/LiteralNodes/IntLiteral.h"
#include "ast/LiteralNodes/StringLiteral.h"
#include "ast/LiteralNodes/BoolLiteral.h"
#include "ast/LiteralNodes/BVLiteral.h"
#include "regex"
#include "ast/UnaryOpNodes/IntToString.h"
#include "ast/UnaryOpNodes/LNot.h"
#include "ast/UnaryOpNodes/BVRedor.h"
#include "ast/UnaryOpNodes/StringLength.h"
#include "ast/UnaryOpNodes/StringToInt.h"
#include "ast/BinaryOpNodes/StringConcat.h"
#include "ast/BinaryOpNodes/LAnd.h"
#include "ast/BinaryOpNodes/LOr.h"
#include "ast/BinaryOpNodes/LXor.h"
#include "ast/BinaryOpNodes/IntAddition.h"
#include "ast/BinaryOpNodes/IntSubstraction.h"
#include "ast/BinaryOpNodes/IntLessThanEq.h"
#include "ast/BinaryOpNodes/IntGreaterThanEq.h"
#include "ast/BinaryOpNodes/IntEquals.h"
#include "ast/BinaryOpNodes/BVSRem.h"
#include "ast/BinaryOpNodes/BVURem.h"
#include "ast/BinaryOpNodes/BVEquals.h"
#include "ast/BinaryOpNodes/BoolEquals.h"
#include "ast/BinaryOpNodes/PrefixOf.h"
#include "ast/BinaryOpNodes/SuffixOf.h"
#include "ast/BinaryOpNodes/Contains.h"
#include "ast/BinaryOpNodes/BVUle.h"
#include "ast/BinaryOpNodes/BVSle.h"
#include "ast/BinaryOpNodes/StringAt.h"
#include "ast/TernaryOpNodes/StringReplace.h"
#include "ast/TernaryOpNodes/StringITE.h"
#include "ast/TernaryOpNodes/IntITE.h"
#include "ast/TernaryOpNodes/BVITE.h"
#include "ast/TernaryOpNodes/Substring.h"
#include "ast/TernaryOpNodes/IndexOf.h"
#include "ast/BinaryOpNodes/BVSlt.h"
#include "ast/BinaryOpNodes/BVUlt.h"
#include "ast/BinaryOpNodes/BVUgt.h"
#include "ast/BinaryOpNodes/BVAnd.h"
#include "ast/BinaryOpNodes/BVOr.h"
#include "ast/BinaryOpNodes/BVShrLogical.h"
#include "ast/BinaryOpNodes/BVShiftLeft.h"
#include "ast/BinaryOpNodes/BVShrArithmetic.h"
#include "ast/BinaryOpNodes/BVXor.h"
#include "ast/BinaryOpNodes/BVSub.h"
#include "ast/BinaryOpNodes/BVSDiv.h"
#include "ast/BinaryOpNodes/BVUDiv.h"
#include "ast/BinaryOpNodes/BVMul.h"
#include "ast/BinaryOpNodes/BVAdd.h"
#include "ast/Blaze/Concat.h"
#include "ast/Blaze/ConstPos.h"
#include "ast/Blaze/ConstStr.h"
#include "ast/Blaze/Pos.h"
#include "ast/Blaze/Str.h"
#include "ast/Blaze/Substr.h"
#include "ast/UnaryOpNodes/BVNeg.h"
#include "ast/UnaryOpNodes/BVNot.h"
#include "ast/VariableNodes/IntVariable.h"
#include "ast/VariableNodes/StringVariable.h"
#include "ast/VariableNodes/BVVariable.h"
#include "ast/VariableNodes/BoolVariable.h"

std::string termToStringWithoutFuncCall(SygusParser::TermContext* constraint, const std::string& funcName) {
    std::stringstream ss;
    bool isFunc = constraint->identifier()->getText() == funcName;
    if(!constraint->term().empty() && !isFunc) ss << "(";
    if(constraint->literal() != nullptr) {
        ss << constraint->literal()->getText();
        return ss.str();
    }
    else {
        ss << constraint->identifier()->getText();
    }
    if(!constraint->term().empty() && !isFunc) {
        for(auto child : constraint->term()) {
            ss << " " << termToStringWithoutFuncCall(child, funcName);
        }
    }
    if(!constraint->term().empty() && !isFunc) ss << ")";
    return ss.str();
}

std::string termToString(SygusParser::TermContext* constraint) {
    std::stringstream ss;
    if(!constraint->term().empty()) ss << "(";
    if(constraint->literal() != nullptr) {
        ss << constraint->literal()->getText();
        return ss.str();
    }
    else {
        ss << constraint->identifier()->getText();
    }

    if(!constraint->term().empty()) {
        for(auto child : constraint->term()) {
            ss << " " << termToString(child);
        }
    }
    if(!constraint->term().empty()) ss << ")";
    return ss.str();
}

std::string defineFunToString(SygusParser::SmtCmdContext* defineFun) {
    std::stringstream ss;
    ss << "(define-fun ";
    std::string funcName = defineFun->Symbol()->getText();
    // func name
    ss << funcName;
    // arg list
    ss << " (";
    std::vector<SygusParser::SortedVarContext*> vars = defineFun->sortedVar();
    for (SygusParser::SortedVarContext* var : vars) {
        std::string varName = var->Symbol()->getText();
        std::string sort = var->sort()->getText();
        if(sort == "(BitVec64)") sort = "(_ BitVec 64)"; // insert space
        ss << "(" << varName << " " << sort << ")";
    }
    ss << ") ";
    // return type
    std::string retType = defineFun->sort()->getText();
    if(retType == "(BitVec64)") retType = "(_ BitVec 64)"; // insert space
    ss << retType << " ";
    // func definition
    SygusParser::TermContext* definition = defineFun->term();
    ss << termToString(definition);
    ss << ")";
    return ss.str();
}


SyGuSFileTask::SyGuSFileTask(const std::string& filename) {
    std::ifstream stream;
    stream.open(filename);

    // Provide the input text in a stream
    input = new antlr4::ANTLRInputStream(stream);

    // Create a lexer from the input
    lexer = new SygusLexer(input);

    // Create a token stream from the lexer
    tokens = new antlr4::CommonTokenStream(lexer);

    // Create a parser from the token stream
    parser = new SygusParser(tokens);

    // Get the parse tree
    parsed = parser->syGuS();

    logic = _getLogic();

    synthFun = _getSynthFun();

    functionName = _getFunctionName();

    functionParameters = _getFunctionParameters();

    functionReturnType = _getFunctionReturnType();

    isPBE = _getIsPBE();

    examples = _getExamples();

    defineFuncs = _getDefineFuncs();

    defineFuncSmtStrings = _getDefineFuncSmtStrings();

    constraintSmtStrings = _getConstraintSmtStrings();

    constraintSmtStringsWithoutFuncCall = _getConstraintSmtStringsWithoutFuncCall();


    std::vector<SygusParser::GroupedRuleListContext *> grammarRules = synthFun->grammarDef()->groupedRuleList();
    std::map<std::string, NonTerminal> nonTerminals;
    for(auto nonTerminal : grammarRules) {
        std::string nontermName = nonTerminal->Symbol()->getSymbol()->getText();
        std::string typeString = nonTerminal->sort()->getText();
        typeString.erase(remove(typeString.begin(), typeString.end(), '('), typeString.end()); //remove '(' from string
        typeString.erase(remove(typeString.begin(), typeString.end(), ')'), typeString.end()); //remove ')' from string
        auto typeOptional = magic_enum::enum_cast<Helper::Type>(typeString);
        assert(typeOptional.has_value() && "could not parse nonterm type");
        Helper::Type type = typeOptional.value();
        nonTerminals.emplace(nontermName, NonTerminal(nontermName, type));
    }

    std::vector<std::shared_ptr<VocabMaker>> vocabMakers;
    for(auto nonTerminal : grammarRules) {
        const NonTerminal& nonTerm = nonTerminals.at(nonTerminal->Symbol()->getText());
        std::vector<SygusParser::GTermContext *> vocabElems;
        for(auto vocabElem : nonTerminal->gTerm()) {
            if(!vocabElem->bfTerm()->bfTerm().empty() || vocabElem->bfTerm()->identifier() == nullptr || !nonTerminals.contains(vocabElem->bfTerm()->identifier()->Symbol()->getText())){
                vocabElems.push_back(vocabElem);
            }
        }
        for(auto vocabElem : vocabElems) {
            vocabMakers.push_back(makeVocabMaker(vocabElem, nonTerm, nonTerminals));
        }
    }
    vocabFactory = std::make_shared<VocabFactory>(vocabMakers);

    defineFuncsMap = _getDefineFuncsMap(vocabMakers);
    isFuncAppConstrType = _getIsFuncAppConstrType();
    if (isFuncAppConstrType) {
        constrFuncName = _getConstrFuncName();
    }
}

Helper::Logic SyGuSFileTask::_getLogic() {
    for(auto command : parsed->cmd()) {
        if(command->smtCmd() != nullptr) {
            auto smtCmd = command->smtCmd();
            if(smtCmd->children.at(1)->getText() == "set-logic") {
                //logic = Helper::StringToLogic(smtCmd->Symbol()->getText());
                return magic_enum::enum_cast<Helper::Logic>(smtCmd->Symbol()->getText()).value();
            }
        }
    }
    assert(false && "no logic found in file");
}

SygusParser::CmdContext *SyGuSFileTask::_getSynthFun() {
    for(auto command : parsed->cmd()) {
        if(command->children.size() > 1 &&  command->children.at(1)->getText() == "synth-fun") {
            return command;
        }
    }
    assert(false && "no synthfun found in file");
}

std::string SyGuSFileTask::_getFunctionName() {
    return synthFun->Symbol(0)->getSymbol()->getText();
}

Helper::Type SyGuSFileTask::_getFunctionReturnType() {
    std::string synthFunSort = synthFun->sort()->getText();
    synthFunSort.erase(remove(synthFunSort.begin(), synthFunSort.end(), '('), synthFunSort.end()); //remove '(' from string
    synthFunSort.erase(remove(synthFunSort.begin(), synthFunSort.end(), ')'), synthFunSort.end()); //remove ')' from string
    return magic_enum::enum_cast<Helper::Type>(synthFunSort).value();
}

std::vector<std::pair<std::string, Helper::Type>> SyGuSFileTask::_getFunctionParameters() {
    std::vector<SygusParser::SortedVarContext*> sortedVars = synthFun->sortedVar();
    std::vector<std::pair<std::string, Helper::Type>> myFunctionParameters;
    for(auto sortedVar : sortedVars) {
        std::string name = sortedVar->Symbol()->getText();
        std::string varSort = sortedVar->sort()->getText();
        varSort.erase(remove(varSort.begin(), varSort.end(), '('), varSort.end()); //remove '(' from string
        varSort.erase(remove(varSort.begin(), varSort.end(), ')'), varSort.end()); //remove ')' from string
        Helper::Type type = magic_enum::enum_cast<Helper::Type>(varSort).value();
        myFunctionParameters.emplace_back(name, type);
    }
    return myFunctionParameters;
}

bool SyGuSFileTask::_getIsPBE() {
    std::vector<SygusParser::TermContext *> constraints = _getConstraints();
    std::string funcName = this->functionName;
    return !constraints.empty() && std::all_of(constraints.begin(), constraints.end(), [funcName] (SygusParser::TermContext* constraint){return isExample(constraint, funcName);});
}

std::vector<SygusParser::TermContext *> SyGuSFileTask::_getConstraints() {
    std::vector<SygusParser::CmdContext *> cmds = parsed->cmd();
    std::vector<SygusParser::TermContext *> constraints;
    for(auto cmd : cmds){
        if(cmd->children.size() > 1 && cmd->children.at(1)->getText() == "constraint") {
            constraints.push_back(cmd->term());
        }
    }
    return constraints;
}

bool SyGuSFileTask::isExample(SygusParser::TermContext *constraint, const std::string& funcName) {
    if(constraint->identifier() != nullptr && constraint->identifier()->getText() != "=") return false;
    SygusParser::TermContext* lhs = constraint->term(0);
    SygusParser::TermContext* rhs = constraint->term(1);
    return (isFuncApp(lhs, funcName) && rhs->literal() != nullptr) || (lhs->literal() != nullptr && isFuncApp(rhs, funcName));
}

bool SyGuSFileTask::_getIsFuncAppConstrType() {
    std::vector<SygusParser::TermContext *> constraints = _getConstraints();
    std::string funcName = this->functionName;
    if (constraints.size() != 1) return false;
    return _getIsFuncAppConstr(constraints[0], funcName).has_value();
}

std::optional<std::string> SyGuSFileTask::_getIsFuncAppConstr(SygusParser::TermContext *constraint,
                                                              const std::string &funcName) {
    if(constraint->identifier() != nullptr && constraint->identifier()->getText() != "=") return std::nullopt;
    SygusParser::TermContext* lhs = constraint->term(0);
    SygusParser::TermContext* rhs = constraint->term(1);
    for (const auto& [name, _] : defineFuncsMap) {
        if ((isFuncApp(lhs, funcName) && isFuncApp(rhs, name)) || (isFuncApp(lhs, name) && isFuncApp(rhs, funcName))) {
            return name;
        }
    }
    return std::nullopt;
}

std::string SyGuSFileTask::_getConstrFuncName() {
    std::vector<SygusParser::TermContext *> constraints = _getConstraints();
    std::string funcName = this->functionName;
    return _getIsFuncAppConstr(constraints[0], funcName).value();
}

std::string SyGuSFileTask::getConstrFuncName() const {
    return constrFuncName;
}

bool SyGuSFileTask::isFuncApp(SygusParser::TermContext *term, const std::string& funcName) {
    return (term->identifier() != nullptr && term->identifier()->Symbol()->getText() == funcName);
}

std::vector<std::shared_ptr<IOExample>> SyGuSFileTask::_getExamples() {
    if(!isPBE) return {};
    std::vector<SygusParser::TermContext *> constraints = _getConstraints();
    std::vector<std::shared_ptr<IOExample>> out;
    out.reserve(constraints.size());
    for(SygusParser::TermContext* constraint : constraints) {
        out.push_back(exampleFromConstraint(constraint));
    }
    return out;
}

std::shared_ptr<IOExample> SyGuSFileTask::exampleFromConstraint(SygusParser::TermContext *constraint) {
    SygusParser::TermContext* lhs = constraint->term(0);
    SygusParser::TermContext* rhs = constraint->term(1);
    if(isFuncApp(lhs, functionName) && rhs->literal() != nullptr) {
        return std::make_shared<IOExample>(lhs, functionParameters, rhs, functionReturnType);
    }
    else if(lhs->literal() != nullptr && isFuncApp(rhs, functionName)) {
        return std::make_shared<IOExample>(rhs, functionParameters, lhs, functionReturnType);
    }
    else {
        BOOST_LOG_TRIVIAL(fatal) << constraint->toString();
        assert(false && "problem with example constraint");
    }
}

Helper::Logic SyGuSFileTask::getLogic() const {
    return logic;
}

const std::string &SyGuSFileTask::getFunctionName() const {
    return functionName;
}

Helper::Type SyGuSFileTask::getFunctionReturnType() const {
    return functionReturnType;
}

const std::vector<std::pair<std::string, Helper::Type>> &SyGuSFileTask::getFunctionParameters() const {
    return functionParameters;
}

bool SyGuSFileTask::isIsFuncAppConstrType() const {
    return isFuncAppConstrType;
}

std::map<std::string, ASTNode *> SyGuSFileTask::getDefineFuncsMap() const {
    return defineFuncsMap;
}

bool SyGuSFileTask::isPbe() const {
    return isPBE;
}

const std::vector<std::shared_ptr<IOExample>> &SyGuSFileTask::getExamples() const {
    return examples;
}

std::shared_ptr<VocabMaker> SyGuSFileTask::makeVocabMaker(SygusParser::GTermContext *vocabElem, const NonTerminal& retType,
                                          const std::map<std::string, NonTerminal>& nonTerminalTypes) {
    if(vocabElem->bfTerm()->literal() != nullptr) { // literal
        variant valVariant = Helper::literalToAny(vocabElem->bfTerm()->literal(), retType.getType());
        int arity = 0;
        std::vector<NonTerminal> childTypes;
        switch (retType.getType()) {
            case Helper::Type::Int: {
                int val = std::get<int>(valVariant);
                return std::make_shared<VocabMaker>(arity, childTypes, retType, Helper::variantToString(val), [val, retType] (const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts){
                    return new IntLiteral(contexts.size() , val, retType);
                });
            }
            case Helper::String: {
                std::string val = std::get<std::string>(valVariant);
                std::string head = '"' + val + '"';
                return std::make_shared<VocabMaker>(arity, childTypes, retType, head, [val, retType] (const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts){
                    return new StringLiteral(contexts.size() , val, retType);
                });
            }
            case Helper::Bool: {
                bool val = std::get<bool>(valVariant);
                return std::make_shared<VocabMaker>(arity, childTypes, retType, Helper::variantToString(val), [val, retType] (const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts){
                    return new BoolLiteral(contexts.size() , val, retType);
                });
            }
            case Helper::BitVec64: {
                uint64_t val = std::get<uint64_t>(valVariant);
                return std::make_shared<VocabMaker>(arity, childTypes, retType, Helper::variantToString(val), [val, retType] (const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts){
                    return new BVLiteral(contexts.size() , val, retType);
                });
            }
        }
    }
    else if (vocabElem->bfTerm()->identifier() != nullptr && vocabElem->bfTerm()->bfTerm().empty()) {
        if(retType.getType() == Helper::Type::Int && std::regex_match(vocabElem->bfTerm()->identifier()->getText(), std::regex("-[1-9][0-9]*"))) { // negative number
            int arity = 0;
            std::vector<NonTerminal> childTypes;
            int val = std::stoi(vocabElem->bfTerm()->identifier()->getText());
            return std::make_shared<VocabMaker>(arity, childTypes, retType, Helper::variantToString(val), [val, retType] (const std::vector<ASTNode*>& children, const std::vector<std::map<std::string, variant>>& contexts){
                return new IntLiteral(contexts.size() , val, retType);
            });
        }
        else {
            std::string varname = vocabElem->bfTerm()->identifier()->Symbol()->getText();
            return variableVocabMaker(retType, varname);
        }
    }
    else {
        std::string funcName = vocabElem->bfTerm()->identifier()->Symbol()->getText();
        int arity = vocabElem->bfTerm()->bfTerm().size();
        std::vector<NonTerminal> childTypes;
        // assumes all args are nonterms
        for(auto child : vocabElem->bfTerm()->bfTerm()) {
            std::string nonterm = child->identifier()->Symbol()->getText();
            childTypes.push_back(nonTerminalTypes.at(nonterm));
        }
        switch (retType.getType()) {
            case(Helper::Type::String): {
                switch (arity) {
                    case 1: {
                        if(funcName == "int.to.str") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IntToString(children.at(0), retType);
                            });
                        }
                        if(funcName == "Str") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new Str(children.at(0), retType);
                            });
                        }
                        if(funcName == "ConstStr") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new ConstStr(children.at(0), retType);
                            });
                        }
                    }
                    case 2: {
                        if (funcName == "str.++") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new StringConcat(children.at(0), children.at(1), retType);
                            });
                        }
                        if (funcName == "Concat") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new Concat(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "str.at") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new StringAt(children.at(0), children.at(1), retType);
                            });
                        }
                    }
                    case 3: {
                        if(funcName == "str.replace") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new StringReplace(children.at(0), children.at(1), children.at(2), retType);
                            });
                        }
                        if(funcName == "ite") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new StringITE(children.at(0), children.at(1), children.at(2), retType);
                            });
                        }
                        if(funcName == "str.substr") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new Substring(children.at(0), children.at(1), children.at(2), retType);
                            });
                        }
                        if(funcName == "SubStr") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new Substr(children.at(0), children.at(1), children.at(2), retType);
                            });
                        }
                    }

                }
            }
            case Helper::Type::Int : {
                switch (arity) {
                    case 1 : {
                        if(funcName == "str.len") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new StringLength(children.at(0), retType);
                            });
                        }
                        if(funcName == "str.to.int") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new StringToInt(children.at(0), retType);
                            });
                        }
                        if(funcName == "ConstPos") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new ConstPos(children.at(0), retType);
                            });
                        }
                    }
                    case 2 : {
                        if (funcName == "+") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IntAddition(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if (funcName == "-") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IntSubstraction(children.at(0), children.at(1), retType);
                            }, false);
                        }
                    }
                    case 3 : {
                        if (funcName == "ite") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IntITE(children.at(0), children.at(1), children.at(2), retType);
                            });
                        }
                        if (funcName == "str.indexof") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IndexOf(children.at(0), children.at(1), children.at(2), retType);
                            });
                        }
                    }
                    case 4: {
                         if (funcName == "Pos") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new Pos(children.at(0), children.at(1), children.at(2), children.at(3), retType);
                            });
                        }
                    }
                }
            }
            case Helper::Type::Bool : {
                switch (arity) {
                    case 1 : {
                        if (funcName == "not") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new LNot(children.at(0), retType);
                            });
                        }
                        if (funcName == "bvredor") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVRedor(children.at(0), retType);
                            });
                        }
                    }
                    case 2 : {
                        if (funcName == "<=") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IntLessThanEq(children.at(0), children.at(1), retType);
                            });
                        }
                        if (funcName == ">=") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new IntGreaterThanEq(children.at(0), children.at(1), retType);
                            });
                        }
                        if (funcName == "=") {
                            if(childTypes.at(0).getType() == childTypes.at(1).getType() && childTypes.at(1).getType() == Helper::Type::Int) {
                                return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                    return new IntEquals(children.at(0), children.at(1), retType);
                                });
                            }
                            if(childTypes.at(0).getType() == childTypes.at(1).getType() && childTypes.at(1).getType() == Helper::Type::BitVec64) {
                                return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                    return new BVEquals(children.at(0), children.at(1), retType);
                                });
                            }
                            if(childTypes.at(0).getType() == childTypes.at(1).getType() && childTypes.at(1).getType() == Helper::Type::Bool) {
                                return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                    return new BoolEquals(children.at(0), children.at(1), retType);
                                });
                            }
                        }
                        if(funcName == "str.prefixof") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new PrefixOf(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "str.suffixof") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new SuffixOf(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "str.contains") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new Contains(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "bvule") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVUle(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "bvsle") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVSle(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "bvslt") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVSlt(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "bvult") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVUlt(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "bvugt") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVUgt(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "and") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new LAnd(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "xor") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new LXor(children.at(0), children.at(1), retType);
                            });
                        }
                        if(funcName == "or") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new LOr(children.at(0), children.at(1), retType);
                            });
                        }
                    }
                }
            }
            case Helper::Type::BitVec64 : {
                switch (arity) {
                    case 1: {
                        if(funcName == "bvnot") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVNot(children.at(0), retType);
                            }, false);
                        }
                        if(funcName == "bvneg") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVNeg(children.at(0), retType);
                            }, false);
                        }
                    }
                    case 2: {
                        if(funcName == "bvand") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVAnd(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvor") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVOr(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvlshr") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVShrLogical(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvshl") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVShiftLeft(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvashr") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVShrArithmetic(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvxor") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVXor(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvsub") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVSub(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvsdiv") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVSDiv(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvudiv") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVUDiv(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvmul") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVMul(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvadd") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVAdd(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvsrem") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVSRem(children.at(0), children.at(1), retType);
                            }, false);
                        }
                        if(funcName == "bvurem") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVURem(children.at(0), children.at(1), retType);
                            }, false);
                        }
                    }
                    case 3 : {
                        if(funcName == "ite") {
                            return std::make_shared<VocabMaker>(arity, childTypes, retType, funcName, [retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                                return new BVITE(children.at(0), children.at(1), children.at(2), retType);
                            }, false);
                        }
                    }
                }
            }
        }
    }
    assert(false && "could not match rule to literal, var, or function");
}

std::shared_ptr<VocabMaker> SyGuSFileTask::variableVocabMaker(const NonTerminal& retType, const std::string& varname) { // hack, for variables the index of the context is given by first child as int
    int arity = 0;
    std::vector<NonTerminal> childTypes;
    switch (retType.getType()) {
        case Helper::Type::Int : {
            return std::make_shared<VocabMaker>(arity, childTypes, retType, varname, [varname, retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
               return new IntVariable(contexts, varname, retType);
            }, true);
        }
        case Helper::String:
            return std::make_shared<VocabMaker>(arity, childTypes, retType, varname, [varname, retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                return new StringVariable(contexts, varname, retType);
            }, true);
        case Helper::Bool:
            return std::make_shared<VocabMaker>(arity, childTypes, retType, varname, [varname, retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                return new BoolVariable(contexts, varname, retType);
            }, true);
        case Helper::BitVec64:
            return std::make_shared<VocabMaker>(arity, childTypes, retType, varname, [varname, retType] (const std::vector<ASTNode*> & children, const std::vector<std::map<std::string, variant>>& contexts) {
                return new BVVariable(contexts, varname, retType);
            }, true);
    }
}

std::vector<std::map<std::string, variant>> SyGuSFileTask::getContexts() {
    std::vector<std::map<std::string, variant>> contexts;
    for(std::shared_ptr<IOExample> example : examples) {
        std::map<std::string, variant> context = example->getInput();
        contexts.push_back(context);
    }
    return contexts;
}

std::vector<variant> SyGuSFileTask::getExpectedValues() {
    std::vector<variant> expectedValues;
    for(std::shared_ptr<IOExample> example : examples) {
        variant val = example->getOutput();
        expectedValues.push_back(val);
    }
    return expectedValues;
}

std::shared_ptr<VocabFactory> SyGuSFileTask::getVocabFactory() const {
    return vocabFactory;
}

std::vector<SygusParser::SmtCmdContext*> SyGuSFileTask::_getDefineFuncs() {
    std::vector<SygusParser::SmtCmdContext*> ret;
    for(auto cmd : parsed->cmd()) {
        if(cmd->smtCmd() != nullptr) {
            if(cmd->smtCmd()->children.at(1)->getText() == "define-fun"){
                ret.push_back(cmd->smtCmd());
            }
        }
    }
    return ret;
}

std::string SyGuSFileTask::_getDefineFuncName(SygusParser::SmtCmdContext *define_func) {
    return define_func->children.at(2)->getText();
}

std::shared_ptr<VocabMaker> SyGuSFileTask::getVocabMaker(const std::vector<std::shared_ptr<VocabMaker>> &vocabMakers,
    const std::string &head) {
    for (const std::shared_ptr<VocabMaker> &vocabMaker: vocabMakers) {
        if (boost::algorithm::to_lower_copy(vocabMaker->getHead()) == boost::algorithm::to_lower_copy(head)) {
            return vocabMaker;
        }
    }
    // quick fix for hd-20-d5-prog.sl -- TODO make more general
    if (head == "#x0000000000000002") {
        uint64_t val = 2;
        Helper::Type type = Helper::Type::BitVec64;
        std::vector<NonTerminal> childTypes;
        NonTerminal retType("Start", type);
        return std::make_shared<VocabMaker>(0, childTypes, retType, Helper::variantToString(val),
                                            [val, retType](const std::vector<ASTNode *> &children,
                                                           const std::vector<std::map<std::string, variant> > &
                                                           contexts) {
                                                return new BVLiteral(contexts.size(), val, retType);
                                            });
    }
    //assert(false);
    return nullptr;
}

ASTNode * SyGuSFileTask::createProg(SygusParser::TermContext *term, const std::vector<std::shared_ptr<VocabMaker>>& vocabMakers) {
    std::shared_ptr<VocabMaker> maker;
    if (term->identifier() != nullptr) {
        maker = getVocabMaker(vocabMakers, term->identifier()->Symbol()->getText());
        if (maker == nullptr) {
            return nullptr;
        }
    }
    else {
        maker = getVocabMaker(vocabMakers, term->literal()->getText());
    }
    std::vector<ASTNode*> children;
    for (int i = 2; i < term->children.size() - 1; i++) {
        auto *child = (SygusParser::TermContext*) term->children.at(i);
        auto prog = createProg(child, vocabMakers);
        if (prog == nullptr) return nullptr;
        children.push_back(prog);
    }
    return maker->apply(children, {});
}

std::map<std::string, ASTNode*> SyGuSFileTask::_getDefineFuncsMap(const std::vector<std::shared_ptr<VocabMaker>>& vocabMakers) {
    std::map<std::string, ASTNode*> ret;
    for(auto defineFunc : _getDefineFuncs()) {
        std::string name = _getDefineFuncName(defineFunc);
        ASTNode* prog = createProg(defineFunc->term(), vocabMakers);
        ret[name] = prog;
    }
    return ret;
}

std::vector<std::string> SyGuSFileTask::_getDefineFuncSmtStrings() {
    std::vector<std::string> out;
    for(auto defineFunc : _getDefineFuncs()) {
        out.push_back(defineFunToString(defineFunc));
    }
    return out;
}

std::vector<std::string> SyGuSFileTask::_getConstraintSmtStrings() {
    std::vector<std::string> out;
    if(!isPBE) {
        std::vector<SygusParser::TermContext *> constraints =  _getConstraints();
        for (auto constraint : constraints) {
            if(!isExample(constraint, functionName)) { // no need to add the examples to the SMT again
                 out.push_back(termToString(constraint));
            }
        }
    }
    return out;
}

const std::vector<std::string> &SyGuSFileTask::getDefineFuncSmtStrings() const {
    return defineFuncSmtStrings;
}

const std::vector<std::string> &SyGuSFileTask::getConstraintSmtStrings() const {
    return constraintSmtStrings;
}

std::vector<std::string> SyGuSFileTask::getConstraintSmtStringsWithoutFuncCall() {
    return constraintSmtStringsWithoutFuncCall;
}

std::vector<std::string> SyGuSFileTask::_getConstraintSmtStringsWithoutFuncCall() {
    std::vector<std::string> out;
    if(!isPBE) {
        std::vector<SygusParser::TermContext *> constraints =  _getConstraints();
        for (auto constraint : constraints) {
            if(!isExample(constraint, functionName)) { // no need to add the examples to the SMT again
                out.push_back(termToStringWithoutFuncCall(constraint, functionName));
            }
        }
    }
    return out;
}