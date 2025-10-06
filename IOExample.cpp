
#include "IOExample.h"

#include <utility>

IOExample::IOExample(const std::map<std::string, variant> &input, variant output) : input(input),
                                                                                             output(std::move(output)) {}

IOExample::IOExample(SygusParser::TermContext* term, std::vector<std::pair<std::string, Helper::Type>> parameters, SygusParser::TermContext* outputTerm, Helper::Type retType): input() {
    std::vector<SygusParser::TermContext*> inputs = term->term();
    assert(inputs.size() == parameters.size() && "amount of inputs must be same as amount of expected parameters");
    for(int i = 0; i < inputs.size(); i++){
        std::string paramName = parameters.at(i).first;
        std::variant paramVal = Helper::literalToAny(inputs.at(i)->literal(), parameters.at(i).second);
        this->input.emplace(paramName, paramVal);
    }
    this->output = Helper::literalToAny(outputTerm->literal(), retType);

}

std::string IOExample::toString() {
    std::stringstream ss;
    ss << "( ";
    std::string padding = "";
    for(auto [paramName, paramValue] : input) {
        ss << padding << paramName << " : " << Helper::variantToString(paramValue);
        padding = ", ";
    }
    ss << " )";
    ss << " : " << Helper::variantToString(output);
    return ss.str();
}

const std::map<std::string, variant> &IOExample::getInput() const {
    return input;
}

const variant &IOExample::getOutput() const {
    return output;
}
