
#ifndef MYSOLVER_SYGUSFILETASK_H
#define MYSOLVER_SYGUSFILETASK_H


#include "Helper.h"
#include "IOExample.h"
#include "VocabFactory.h"
#include "SygusLexer.h"
#include "NonTerminal.h"

class SyGuSFileTask {
    // antlr classes
    antlr4::ANTLRInputStream* input;
    SygusLexer* lexer;
    antlr4::CommonTokenStream* tokens;
    SygusParser* parser;

    /**
     * parsed object from ant lr
     */
    SygusParser::SyGuSContext* parsed;

    /**
     * logic specified in file
     */
    Helper::Logic logic;

    /**
     * parses logic
     * @return logic specified in file
     */
    Helper::Logic _getLogic();

    /**
     * name of function to synthesize
     */
    std::string functionName;

    /**
    * parses name of function to synthesize
    * @return name of function to synthesized as specified in file
    */
    std::string _getFunctionName();

    /**
     * return type of function to synthesize
     */
    Helper::Type functionReturnType;

    /**
     * parses return type of function to synthesize
     * @return return type of function to synthesize as specified in file
     */
    Helper::Type _getFunctionReturnType();

    /**
     * parameters of function to synthesize
     */
    std::vector<std::pair<std::string, Helper::Type>> functionParameters;

    /**
     * parameters of function to synthesize
     * @return parameters of function to synthesize as specified in file
     */
    std::vector<std::pair<std::string, Helper::Type>> _getFunctionParameters();

    /**
     * whether the synthesis task is a programming by example task
     */
    bool isPBE;

    /**
     * parses isPBE
     * @return returns true, iff examples are specified in file
     */
    bool _getIsPBE();

    /**
     * whether the specification is given through a function
     */
    bool isFuncAppConstrType;

    /**
     *
     * @return returns whether the specification is given through a function
     */
    bool _getIsFuncAppConstrType();

    /**
     * checks whether constraint is a function application
     * @param constraint constraint to check
     * @param funcName function name
     * @return whether constraint is function application
     */
    std::optional<std::string> _getIsFuncAppConstr(SygusParser::TermContext *constraint, const std::string &funcName);

    /**
     * name of function in constraint
     */
    std::string constrFuncName;

    /**
     *
     * @return name of function in constraint
     */
    std::string _getConstrFuncName();

public:
    [[nodiscard]] std::string getConstrFuncName() const;

private:
    /**
     * the define functions
     */
    std::vector<std::string> defineFuncSmtStrings;
    /**
    *
    * @return parses the define-func strings
    */
    std::vector<std::string> _getDefineFuncSmtStrings();

    /**
     * the constraints of the synthesis problem
     */
    std::vector<std::string> constraintSmtStrings;
    /**
     * parses the constraints of the synth problem
     * @return the constraint smt strings of the file
     */
    std::vector<std::string> _getConstraintSmtStrings();

    /**
     * the constraint smt strings without a function call
     */
    std::vector<std::string> constraintSmtStringsWithoutFuncCall;
    /**
     * parses the constraint smt strings from the file
     * @return the constraint smt strings without a function call
     */
    std::vector<std::string> _getConstraintSmtStringsWithoutFuncCall();

    /**
     * define functions from file
     */
    std::vector<SygusParser::SmtCmdContext*> defineFuncs;
    /**
     * parses the file for define functions
     * @return the define functions of the smt file
     */
    std::vector<SygusParser::SmtCmdContext*> _getDefineFuncs();

    /**
     *
     * @param define_func define-func expression
     * @return name of function in define-func expression
     */
    std::string _getDefineFuncName(SygusParser::SmtCmdContext * define_func);

    /**
     *
     * @param vocabMakers all vocabmakers
     * @param head head of wanted vocabmaker
     * @return vocabmaker for given head
     */
    static std::shared_ptr<VocabMaker> getVocabMaker(const std::vector<std::shared_ptr<VocabMaker>> & vocabMakers, const std::string & head);

    /**
     * creates prog from term
     * @param term term for which to create prog
     * @param vocabMakers all vocab makers
     * @return AST program that represents the given term
     */
    ASTNode * createProg(SygusParser::TermContext * term, const std::vector<std::shared_ptr<VocabMaker>>& vocabMakers);

    /**
     * map from name of define func to their implementation
     */
    std::map<std::string, ASTNode*> defineFuncsMap;

    /**
     *
     * @return map from name of define func to their implementation
     */
    std::map<std::string, ASTNode *> _getDefineFuncsMap(const std::vector<std::shared_ptr<VocabMaker>>&);

    /**
     * parses file for constraints
     * @return constraints of synthesis file
     */
    std::vector<SygusParser::TermContext *> _getConstraints();

    /**
     * io examples from the file
     */
    std::vector<std::shared_ptr<IOExample>> examples;

    /**
     * parses the io examples
     * @return examples from file
     */
    std::vector<std::shared_ptr<IOExample>> _getExamples();

    /**
     * transforms the constraint into an IOExample
     * @param constraint
     * @return example generated from constraint
     */
    std::shared_ptr<IOExample> exampleFromConstraint(SygusParser::TermContext* constraint);

    /**
     * checks whether constraint is an example
     * @param constraint
     * @param funcName
     * @return true iff constraint is an example
     */
    static bool isExample(SygusParser::TermContext* constraint, const std::string& funcName);

    /**
     * checks whether term is a function application
     * @param term
     * @param funcName
     * @return true, iff term is function application
     */
    static bool isFuncApp(SygusParser::TermContext* term, const std::string& funcName);

    /**
     * function to synthesize
     */
    SygusParser::CmdContext* synthFun;

    /**
     *
     * @return parses function to synthesize
     */
    SygusParser::CmdContext* _getSynthFun();

    /**
     * vocabfactory that is created during parsing
     */
    std::shared_ptr<VocabFactory> vocabFactory;

    /**
     * creates vocabmaker for given element
     * @param vocabElem grammar element
     * @param retType return type of vocabmaker
     * @param nonTerminalTypes map from strings to nonterminals
     * @return vocabmaker for given element
     */
    static std::shared_ptr<VocabMaker> makeVocabMaker(SygusParser::GTermContext* vocabElem, const NonTerminal& retType, const std::map<std::string, NonTerminal>& nonTerminalTypes);

    /**
     * creates variable vocab maker
     * @param retType return type for vocabmaker
     * @param varname name of variable to create vocab maker for
     * @return variable vocab maker
     */
    static std::shared_ptr<VocabMaker> variableVocabMaker(const NonTerminal& retType, const std::string& varname);

public:
    /**
     *
     * @return define func smt strings
     */
    [[nodiscard]] const std::vector<std::string> &getDefineFuncSmtStrings() const;

    /**
     *
     * @return constraint smt strings
     */
    [[nodiscard]] const std::vector<std::string> &getConstraintSmtStrings() const;

    /**
     *
     * @return logic of input file
     */
    [[nodiscard]] Helper::Logic getLogic() const;

    /**
     *
     * @return name of function to synthesize
     */
    [[nodiscard]] const std::string &getFunctionName() const;

    /**
     *
     * @return return type of fucntion to synthesize
     */
    [[nodiscard]] Helper::Type getFunctionReturnType() const;

    /**
     *
     * @return function paramaters as pairs of name and type
     */
    [[nodiscard]] const std::vector<std::pair<std::string, Helper::Type>> &getFunctionParameters() const;

    /**
     *
     * @return whether specification is given through a function
     */
    [[nodiscard]] bool isIsFuncAppConstrType() const;

    /**
     *
     * @return ast representations of defined functions
     */
    [[nodiscard]] std::map<std::string, ASTNode *> getDefineFuncsMap() const;

    /**
     *
     * @return true iff file describes a pbe problem
     */
    [[nodiscard]] bool isPbe() const;

    /**
     *
     * @return examples specified in file
     */
    [[nodiscard]] const std::vector<std::shared_ptr<IOExample>> &getExamples() const;

    /**
     *
     * @param filename file to parse
     */
    explicit SyGuSFileTask(const std::string& filename);

    /**
     *
     * @return vocab factory containing vocabmakers for the grammar specified in file
     */
    [[nodiscard]] std::shared_ptr<VocabFactory> getVocabFactory() const;

    /**
     *
     * @return input examples, as map from input variable name to value
     */
    std::vector<std::map<std::string, variant>> getContexts();

    /**
     *
     * @return output examples
     */
    std::vector<variant> getExpectedValues();

    /**
     *
     * @return constraint smt strings without a function call
     */
    std::vector<std::string> getConstraintSmtStringsWithoutFuncCall();
};


#endif //MYSOLVER_SYGUSFILETASK_H
