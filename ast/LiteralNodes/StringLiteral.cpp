
#include "StringLiteral.h"
#include "../Blaze/Pos.h"

StringLiteral::StringLiteral(int numContexts, std::string value, const NonTerminal& type) : LiteralNode(numContexts, value, type) {
    head = '"' + value + '"';
    cost = 10;
    if(type.getName() == "ntToken") {
        int token = Pos::nameToToken.at(value);
        cost = Pos::tokenToCost.at(token);
    }
}
