
#ifndef MYSOLVER_VARIABLENODE_H
#define MYSOLVER_VARIABLENODE_H


#include "ASTNode.h"

template<class T>
class VariableNode : public ASTNode {
    std::vector<std::map<std::string, variant>> contexts;
protected:
    void initValues(){
        for(std::map<std::string, variant> context : contexts) {
            T val = std::get<T>(context.at(head)); // get value of our var
            values.push_back(val);
        }
    }
public:
    VariableNode(const std::vector<std::map<std::string, variant>> &contexts, const NonTerminal& type) : contexts(contexts), ASTNode(type) {
        height = 0;
        terms = 1;
        cost = 0;
    }

    bool includes(std::string varName) override {
        return head == varName;
    }

    std::vector<variant> eval(const std::vector<std::map<std::string, variant>> &contextsArg) const override {
        std::vector < variant > out;
        for(std::map<std::string, variant> context : contextsArg) {
            T val = std::get<T>(context.at(head)); // get value of our var
            out.push_back(val);
        }
        return out;
    }
};


#endif //MYSOLVER_VARIABLENODE_H
