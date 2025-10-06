
#ifndef MYSOLVER_UNARYOPNODE_H
#define MYSOLVER_UNARYOPNODE_H

#include "ASTNode.h"

template<class T>
class UnaryOpNode : public ASTNode{
    virtual T doOp(variant input) const = 0;

protected:
    void initValues() {
        // assign values
        const std::vector<variant>& argVals = children.at(0)->getValues();
        for(variant val : argVals) {
            values.push_back(doOp(val));
        }
    }

public:
    UnaryOpNode(ASTNode* arg, const NonTerminal& type) : ASTNode(type) {
        children.push_back(arg);
        height = arg->getHeight() + 1;
        terms = arg->getTerms() + 1;

        cost = arg->getCost() + 1;
    }

    bool includes(std::string varName) override {
        return children.at(0)->includes(varName);
    };

    [[nodiscard]] std::vector<variant> eval(const std::vector<std::map<std::string, variant>> &contexts) const override {
        std::vector<variant> out;
        const std::vector<variant>& argVals = children.at(0)->eval(contexts);
        for(variant val : argVals) {
            out.push_back(doOp(val));
        }
        return out;
    }
};


#endif //MYSOLVER_UNARYOPNODE_H
