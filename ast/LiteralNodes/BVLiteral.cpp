#include <format>
#include "BVLiteral.h"

BVLiteral::BVLiteral(int numContexts, uint64_t value, const NonTerminal& type) : LiteralNode(numContexts, value, type) {
    head = Helper::bvToSmtString(value);
}