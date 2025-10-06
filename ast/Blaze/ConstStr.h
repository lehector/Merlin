
#ifndef CONSTSTR_H
#define CONSTSTR_H
#include "../UnaryOpNode.h"


class ConstStr : public UnaryOpNode<std::string> {
    std::string doOp(variant input) const override;
public:
    explicit ConstStr(ASTNode* arg, const NonTerminal& type);
    static std::string doOpStatic(variant input);
};



#endif //CONSTSTR_H
