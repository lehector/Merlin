
#include "Concat.h"

#include "Substr.h"

std::string Concat::doOp(variant str1, variant str2) const {
    return doOpStatic(str1, str2);
}

std::string Concat::doOpStatic(variant str1, variant str2) {
    const std::string& str1Val = std::get<std::string>(str1);
    const std::string& str2Val = std::get<std::string>(str2);
    if(str1Val == ERRSTRING || str2Val == ERRSTRING) return ERRSTRING;
    return str1Val + str2Val;
}
