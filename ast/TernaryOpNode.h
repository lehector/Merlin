
#ifndef MYSOLVER_TERNARYOPNODE_H
#define MYSOLVER_TERNARYOPNODE_H

#include "ASTNode.h"

template<class T>
class TernaryOpNode : public ASTNode{
    virtual T doOp(variant a0, variant a1, variant a2) const = 0;

protected:
    void initValues() {
        // assign values
        const std::vector<variant>& a0Vals = children.at(0)->getValues();
        const std::vector<variant>& a1Vals = children.at(1)->getValues();
        const std::vector<variant>& a2Vals = children.at(2)->getValues();
        assert(a0Vals.size() == a1Vals.size() && a1Vals.size() == a2Vals.size());
        for(int i = 0; i < a0Vals.size(); i++) {
            values.push_back(doOp(a0Vals.at(i), a1Vals.at(i), a2Vals.at(i)));
        }
    }

public:
    TernaryOpNode(ASTNode* arg0, ASTNode* arg1, ASTNode* arg2, const NonTerminal& type) : ASTNode(type) {
        // assign children
        children.push_back(arg0);
        children.push_back(arg1);
        children.push_back(arg2);

        height = 1 + std::max({arg0->getHeight(), arg1->getHeight(), arg2->getHeight()});

        terms = 1 + arg0->getTerms() + arg1->getTerms() + arg2->getTerms();

        cost = 1 + arg0->getCost() + arg1->getCost() + arg2->getCost();
    }

    bool includes(std::string varName) override {
        return children.at(0)->includes(varName) || children.at(1)->includes(varName) || children.at(2)->includes(varName);
    }

    std::vector<variant> eval(const std::vector<std::map<std::string, variant>> &contexts) const override {
        std::vector<variant> out;
        const std::vector<variant>& a0Vals = children.at(0)->eval(contexts);
        const std::vector<variant>& a1Vals = children.at(1)->eval(contexts);
        const std::vector<variant>& a2Vals = children.at(2)->eval(contexts);
        assert(a0Vals.size() == a1Vals.size() && a1Vals.size() == a2Vals.size());
        for(int i = 0; i < a0Vals.size(); i++) {
            out.push_back(doOp(a0Vals.at(i), a1Vals.at(i), a2Vals.at(i)));
        }
        return out;
    }

};


#endif //MYSOLVER_TERNARYOPNODE_H
