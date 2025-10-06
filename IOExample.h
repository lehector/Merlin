
#ifndef MYSOLVER_IOEXAMPLE_H
#define MYSOLVER_IOEXAMPLE_H


#include <map>
#include <string>
#include "SygusParser.h"
#include "Helper.h"

class IOExample {
    /**
     * input context
     */
    std::map<std::string, variant> input;
    /**
     * expected output
     */
    variant output;
public:
    /**
     *
     * @param input input context
     * @param output expected output
     */
    IOExample(const std::map<std::string, variant> &input, variant output);

    /**
     *
     * @return input context
     */
    [[nodiscard]] const std::map<std::string, variant> &getInput() const;

    /**
     *
     * @return expected output
     */
    [[nodiscard]] const variant &getOutput() const;

    /**
     * constructor for parser
     * @param term
     * @param parameters
     * @param output
     * @param retType
     */
    IOExample(SygusParser::TermContext* term, std::vector<std::pair<std::string, Helper::Type>> parameters, SygusParser::TermContext* output, Helper::Type retType);

    /**
     *
     * @return string representation of example
     */
    std::string toString();
};


#endif //MYSOLVER_IOEXAMPLE_H
