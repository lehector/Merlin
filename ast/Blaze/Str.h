
#ifndef STR_H
#define STR_H
#include "../UnaryOpNode.h"


class Str : public UnaryOpNode<std::string>{
public:
    explicit Str(ASTNode* arg, const NonTerminal& type);
    static std::string doOpStatic(variant input);

private:
    [[nodiscard]] std::string doOp(variant input) const override;
};



#endif //STR_H
