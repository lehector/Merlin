
#include "Pos.h"
#define ERROR_INT_CONST INT_MIN

std::unordered_map<int, std::regex> Pos::tokenToPattern = {
    {0, std::regex("[A-Z][a-z]+")}, // ProperCase
    {1, std::regex("[A-Z]+")}, // CAPS
    {2, std::regex("[a-z]+")}, // lowercase
    {3, std::regex("\\d+")}, // Digits
    //{4, std::regex("[\\w&&\\D]+")}, // Alphabets
    {4, std::regex("[a-zA-Z_]+")}, // Alphabets
    {5, std::regex("[\\w]+")}, // Alphanumeric
    {6, std::regex("\\s+")}, // Whitespace
    {7, std::regex("^")}, // StartT
    {8, std::regex("$")}, // EndT
    {9, std::regex("([A-Z][a-z]+)((\\s+)([A-Z][a-z]+))*")}, // ProperCaseWSpaces
    {10, std::regex("([A-Z]+)((\\s+)([A-Z]+))*")}, // CapsWSpaces
    {11, std::regex("([a-z]+)((\\s+)([a-z]+))*")}, // lowercaseWSpaces
    //{12, std::regex("([\\w&&\\D]+)((\\s+)([\\w&&\\D]+))*")}, // AlphabetWSpaces
    {12, std::regex("([a-zA-Z_]+)((\\s+)([a-zA-Z_]+))*")}, // AlphabetWSpaces
    {13, std::regex("[|]")}, // Sep
};
std::unordered_map<int, std::string> Pos::tokenToName = {
    {0, "ProperCase"}, // ProperCase
    {1, "CAPS"}, // CAPS
    {2, "lowercase"}, // lowercase
    {3, "Digits"}, // Digits
    {4, "Alphabets"}, // Alphabets
    {5, "Alphanumeric"}, // Alphanumeric
    {6, "Whitespace"}, // Whitespace
    {7, "StartT"}, // StartT
    {8, "EndT"}, // EndT
    {9, "ProperCaseWSpaces"}, // ProperCaseWSpaces
    {10, "CapsWSpaces"}, // CapsWSpaces
    {11, "lowercaseWSpaces"}, // lowercaseWSpaces
    {12, "AlphabetWSpaces"}, // AlphabetWSpaces
    {13, "Sep"}, // Sep
};

std::unordered_map<int, int> Pos::tokenToCost = {
    {0,10}, // ProperCase
    {1,10}, // CAPS
    {2,10}, // lowercase
    {3,10}, // Digits
    {4,20}, // Alphabets
    {5,20}, // Alphanumeric
    {6,20}, // Whitespace
    {7,20}, // StartT
    {8,20}, // EndT
    {9,50}, // ProperCaseWSpaces
    {10,50}, // CapsWSpaces
    {11,50}, // lowercaseWSpaces
    {12,20}, // AlphabetWSpaces
    {13,0} // Sep
};

std::unordered_map<std::string, int> Pos::nameToToken = {
    {"ProperCase", 0}, // ProperCase
    {"CAPS", 1}, // CAPS
    {"lowercase", 2}, // lowercase
    {"Digits", 3}, // Digits
    {"Alphabets", 4}, // Alphabets
    {"Alphanumeric", 5}, // Alphanumeric
    {"Whitespace", 6}, // Whitespace
    {"StartT", 7}, // StartT
    {"EndT", 8}, // EndT
    {"ProperCaseWSpaces", 9}, // ProperCaseWSpaces
    {"CapsWSpaces", 10}, // CapsWSpaces
    {"lowercaseWSpaces", 11}, // lowercaseWSpaces
    {"AlphabetWSpaces", 12}, // AlphabetWSpaces
    {"Sep", 13}, // Sep
};

Pos::Pos(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, ASTNode* arg3, const NonTerminal &type) : QuadraryOpNode(arg0, arg1, arg2, arg3, type){
    head = "Pos";
    initValues();
}

int Pos::doOpStatic(variant a0, variant a1, variant a2, variant a3) {
    std::string string = std::get<std::string>(a0);
    std::string token = std::get<std::string>(a1);
    int k = std::get<int>(a2);
    int d = std::get<int>(a3);


    const std::regex& pattern = tokenToPattern.at(nameToToken.at(token));
    std::smatch matcher;
    std::vector<std::pair<int, int>> list;

    if(token == "StartT") {
        list.emplace_back(0, 0);
    }
    else if(token == "EndT") {
        list.emplace_back(string.length(), string.length());
    }
    else {
        std::smatch match;
        int offset = 0;
        while (std::regex_search(string, match, pattern)) {
            int start = offset + match.position(0);
            int end = start + match.length(0);
            offset = end;
            list.emplace_back(start, end);
            string = match.suffix();
        }
    }

    assert(k != 0);
    if (k > 0) k--;
    if (k < 0) {
        k = list.size() + k;
    }
    if(k >= 0 && k < list.size()) {
        if(d == 0) {
            int start = list.at(k).first;
            return start;
        }
        else {
            int end = list.at(k).second;
            return end;
        }
    }
    else {
        return ERROR_INT_CONST;
    }

}

int Pos::doOp(variant a0, variant a1, variant a2, variant a3) const {
    return doOpStatic(a0, a1, a2, a3);
}
