
#ifndef POS_H
#define POS_H
#include <regex>

#include "../QuadraryOpNode.h"

/*
 *
 * WARNING: THERE IS A DISCREPENCY IN REGEX FLAVOURS TO JAVA, FOR EXAMPLE ALPHABETS TOKEN,
 * changed to how I think it is correct
 *
 */

class Pos : public QuadraryOpNode<int>{
public:
    Pos(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, ASTNode* arg3, const NonTerminal &type);
    static int doOpStatic(variant a0, variant a1, variant a2, variant a3);

    static std::unordered_map<std::string, int> nameToToken;
    static std::unordered_map<int, int> tokenToCost;

private:
    int doOp(variant a0, variant a1, variant a2, variant a3) const override;

    static std::unordered_map<int, std::regex> tokenToPattern;
    static std::unordered_map<int, std::string> tokenToName;
};



#endif //POS_H
