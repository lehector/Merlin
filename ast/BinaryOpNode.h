
#ifndef MYSOLVER_BINARYOPNODE_H
#define MYSOLVER_BINARYOPNODE_H


#include "ASTNode.h"

template<class T>
class BinaryOpNode : public ASTNode{
    virtual T doOp(variant l, variant r) const = 0;

protected:
    void initValues() {
        // assign values
        const std::vector<variant>& lhsVals = children.at(0)->getValues();
        const std::vector<variant>& rhsVals = children.at(1)->getValues();
        assert(lhsVals.size() == rhsVals.size());
        for(int i = 0; i < lhsVals.size(); i++) {
            values.push_back(doOp(lhsVals.at(i), rhsVals.at(i)));
        }
    };

public:
    BinaryOpNode(ASTNode* lhs, ASTNode* rhs, const NonTerminal& type) : ASTNode(type) {
        // assign children
        children.push_back(lhs);
        children.push_back(rhs);
        height = 1 + std::max({lhs->getHeight(), rhs->getHeight()});
        terms = 1 + lhs->getTerms() + rhs->getTerms();
        cost = 1 + lhs->getCost() + rhs->getCost();
    }

    bool includes(std::string varName) override {
        return children.at(0)->includes(varName) || children.at(1)->includes(varName);
    }

    [[nodiscard]] std::vector<variant> eval(const std::vector<std::map<std::string, variant>> &contexts) const override {
        std::vector<variant> out;
        const std::vector<variant>& lhsVals = children.at(0)->eval(contexts);
        const std::vector<variant>& rhsVals = children.at(1)->eval(contexts);
        assert(lhsVals.size() == rhsVals.size());
        for(int i = 0; i < lhsVals.size(); i++) {
            out.push_back(doOp(lhsVals.at(i), rhsVals.at(i)));
        }
        return out;
    }
};


#endif //MYSOLVER_BINARYOPNODE_H
