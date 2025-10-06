
#ifndef MYSOLVER_LITERALNODE_H
#define MYSOLVER_LITERALNODE_H


#include "ASTNode.h"

template<class T>
class LiteralNode : public ASTNode{
protected:
    T value;
public:
    bool includes(std::string varName) override {
        return false;
    }

    LiteralNode(int numContexts, T value, const NonTerminal& type): value(value), ASTNode(type) {
        height = 0;
        terms = 1;
        for(int i = 0; i < numContexts; i++){
            values.push_back(value);
        }
        cost = 0;
    }

    [[nodiscard]] std::vector<variant> eval(const std::vector<std::map<std::string, variant>> &contexts) const override {
        std::vector<variant> out;
        for(int i = 0; i < contexts.size(); i++) {
            out.push_back(value);
        }
        return out;
    }
};


#endif //MYSOLVER_LITERALNODE_H
