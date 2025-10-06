
#include "BoolLiteral.h"

BoolLiteral::BoolLiteral(int numContexts, bool value, const NonTerminal& type) : LiteralNode(numContexts, value, type) {
    head = value ? "true" : "false";
}
