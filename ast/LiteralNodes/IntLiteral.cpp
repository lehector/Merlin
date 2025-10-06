
#include "IntLiteral.h"

IntLiteral::IntLiteral(int numContexts, int value, const NonTerminal& type) : LiteralNode(numContexts, value, type) {
    head = std::to_string(value);
    if(type.getName() == "ntConstPos") {
        cost = 100;
    }
    if(type.getName() == "ntDir") {
        cost = 0;
    }
    if(type.getName() == "ntInt") {
        switch(value) {
            case 1: {
                cost = 0;
                break;
            }
            case 2: {
                cost = 1;
                break;
            }
            case 3: {
                cost = 2;
                break;
            }
            case -1: {
                cost = 2;
                break;
            }
            case -2: {
                cost = 3;
                break;
            }
            case -3: {
                cost = 4;
                break;
            }
        }
    }
}
