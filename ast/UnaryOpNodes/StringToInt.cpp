
#include "StringToInt.h"

StringToInt::StringToInt(ASTNode* arg, const NonTerminal& type) : UnaryOpNode(arg, type) {
    head = "str.to.int";
    initValues();
}

int StringToInt::doOp(variant input) const {
    return doOpStatic(input);
}

int StringToInt::doOpStatic(variant input) {
    std::string val = std::get<std::string>(input);
    try {
        int out = std::stoi(val);
        return out >= 0 ? out : -1;
    }
    catch (std::invalid_argument e) {
        return -1;
    }
    catch (std::out_of_range e) {
        return -1;
    }
}
