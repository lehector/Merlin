
#include "Substr.h"
#include "../TernaryOpNodes/Substring.h"

std::string Substr::doOp(variant a0, variant a1, variant a2) const {
    return doOpStatic(a0, a1, a2);
}

std::string Substr::doOpStatic(variant a0, variant a1, variant a2) {
    /*
     * not necessary in this grammar
    std::string x = std::get<std::string>(a0);
    if(x == ERRSTRING) return x;
    */
    int pos1 = std::get<int>(a1);
    int pos2 = std::get<int>(a2);
    if(pos1 < 0 || pos2 < 0 || pos1 > pos2) return ERRSTRING;
    int len = pos2 - pos1;
    return Substring::doOpStatic(a0, pos1, len);
}

Substr::Substr(ASTNode*arg0, ASTNode*arg1,
    ASTNode*arg2, const NonTerminal &type) : TernaryOpNode(arg0, arg1, arg2, type) {
    head = "SubStr";
    initValues();
}
