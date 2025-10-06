
#ifndef MYSOLVER_SMTPROCESS_H
#define MYSOLVER_SMTPROCESS_H

#include "tuple"
#include "vector"
#include "string"
#include "../SyGuSFileTask.h"

/**
 * handles smt process
 */
class SMTProcess {
protected:
    /**
     * pointer to parser object
     */
    SyGuSFileTask* syGuSFileTask;
    /**
     * name of the function to synthesize
     */
    std::string functionName;
    /**
     * return type of function to synthesize
     */
    Helper::Type returnType;
    //std::unordered_map<std::string, Helper::Type> functionParameters;
    /**
     * parameters of function to synthesize
     */
    std::vector<std::pair<std::string, Helper::Type>> functionParameters;
    /**
     * define-func strings in smt / sygus file
     */
    std::vector<std::string> defineFuncStrings;

    /**
     * head of the smt file to generate
     */
    std::string fileHead;
    /**
     * foot of the smt file to generate
     */
    std::string fileFoot;
    /**
     * const declarations for argument symbols
     */
    std::string functionParamConstDeclarations;

    /**
     * prefix of smt query
     */
    std::string checkSMTPrefix;
    /**
     * postfix of smt query
     */
    std::string checkSMTPostfix;

    /**
     * prefix of generating output example query
     */
    std::string createSMTOutputPrefix;
    /**
     * postfix of generating output example query
     */
    std::string createSMTOutputPostfix;

    /**
     *
     * @return pre and postfix for generating output example query
     */
    std::pair<std::string, std::string> createSMTGetOutputPrePostFix();

    /**
     *
     * @param contexts input examples
     * @return creates smt string to generate output example
     */
    std::string createSMTStringToReceiveExOutputWithCurrentContexts(const std::vector<std::map<std::string, variant>>& contexts);


public:
    /**
     *
     * @return smt query pre and postfix
     */
    std::pair<std::string, std::string> createSMTStrings();
    /**
    * @param context input examples
    * @return creates smt string to generate output example
    */
    std::string createSMTStringToReceiveExOutput(const std::map<std::string, variant>& context);

    /**
     * checks whether the program is correct
     * @param programCode code of program to check correctenss for
     * @return optional. if program is not correct, the optional returns a new IO Example
     */
    virtual std::optional<std::unique_ptr<IOExample>> checkValidity(std::string programCode) = 0;

    /**
     *
     * @param currentContexts current input examples
     * @return new output example
     */
    virtual std::unique_ptr<IOExample> getExample(const std::vector<std::map<std::string, variant>>& currentContexts) = 0;

    explicit SMTProcess(SyGuSFileTask *syGuSFileTask);
};


#endif //MYSOLVER_SMTPROCESS_H
