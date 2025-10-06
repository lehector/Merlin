#include <mata/nfa/nfa.hh>

#include "../Helper.h"
#include "gtest/gtest.h"
#include "../LevenshteinAutomatonCreator.h"
#include "../SyGuSFileTask.h"
#include "../ast/VariableNode.h"
#include "../ast/BinaryOpNodes/BVAdd.h"
#include "../ast/BinaryOpNodes/BVAnd.h"
#include "../ast/BinaryOpNodes/BVMul.h"
#include "../ast/BinaryOpNodes/BVOr.h"
#include "../ast/BinaryOpNodes/BVSDiv.h"
#include "../ast/BinaryOpNodes/BVSRem.h"
#include "../ast/BinaryOpNodes/BVSub.h"
#include "../ast/BinaryOpNodes/BVXor.h"
#include "../ast/LiteralNodes/BVLiteral.h"
#include "../ast/UnaryOpNodes/BVNeg.h"
#include "../ast/UnaryOpNodes/BVNot.h"
#include "../ast/VariableNodes/BVVariable.h"

TEST(HELPERSUITE, countTrailingZeros) {
    uint64_t noZeros = 0x000000000000000f;
    int numZeros = Helper::countTrailingZeros(noZeros);
    ASSERT_EQ(numZeros, 0);
    uint64_t threeZeros = 0x0000000000000008;
    numZeros = Helper::countTrailingZeros(threeZeros);
    ASSERT_EQ(numZeros, 3);
    uint64_t eightZeros = 0x0000000000000100;
    numZeros = Helper::countTrailingZeros(eightZeros);
    ASSERT_EQ(numZeros, 8);
}

TEST(HELPERSUITE, mytest) {
    uint64_t prod = 0xffc55eea2b0c2c3d;
    uint64_t mul1 = 0x000000003757a7ff;
    uint64_t mul2 = 0xfffffffffef0cbc3;
    ASSERT_EQ(prod, mul1 * mul2);
    ASSERT_LE(Helper::countTrailingZeros(mul2), Helper::countTrailingZeros(prod) - Helper::countTrailingZeros(mul1));
}

TEST(HELPERSUITE, LevenshteinAutomatonTest) {
    std::string val = "0+106 769-858-438";
    std::string target =  "+106 (769) 858-438";
    std::set<char> alphabet;
    for(char c : val) {
        alphabet.insert(c);
    }
    for(char c : target) {
        alphabet.insert(c);
    }
    LevenshteinAutomatonCreator creator(alphabet, 4, target);
    mata::nfa::Nfa nfa = creator.createLevenshteinAutomaton();
    mata::Word word;
    for(char c : val) {
        word.push_back(c);
    }
    ASSERT_TRUE(nfa.is_in_lang(word));
}

TEST(HELPERSUITE, LastProblemTrace) {
    std::string filename = "../../benchmarks/deobfusc/target_217.sl";
    SyGuSFileTask sFileTask(filename);
    std::vector<std::map<std::string, variant> > contexts = sFileTask.getContexts();
    std::vector<variant> expectedValues = sFileTask.getExpectedValues();
    NonTerminal dummyNonTerm("dummy", Helper::Type::BitVec64);
    auto b = new BVVariable(contexts, "b", dummyNonTerm);
    auto a = new BVVariable(contexts, "a", dummyNonTerm);
    auto d = new BVVariable(contexts, "d", dummyNonTerm);
    auto n1 = new BVXor(a, d, dummyNonTerm);
    auto n2 = new BVSub(b, n1, dummyNonTerm);
    auto n3 = new BVNot(b, dummyNonTerm);
    auto n4 = new BVAdd(a, d, dummyNonTerm);
    auto n5 = new BVOr(n3, n4, dummyNonTerm);
    auto n6 = new BVSub(n2, n5, dummyNonTerm);
    auto n7 = new BVMul(b, n6, dummyNonTerm);

    auto n8 = new BVMul(b, a, dummyNonTerm);
    auto n9 = new BVSub(n8, a, dummyNonTerm);
    auto all = new BVAnd(n7, n9, dummyNonTerm);

    std::cout << all->getTrace(expectedValues);
    ASSERT_EQ(all->getValues(), expectedValues);
}

TEST(HELPERSUITE, LastProblemTrace2) {
    std::string filename = "../../benchmarks/deobfusc/target_217.sl";
    SyGuSFileTask sFileTask(filename);
    std::vector<std::map<std::string, variant> > contexts = sFileTask.getContexts();
    std::vector<variant> expectedValues = sFileTask.getExpectedValues();
    NonTerminal dummyNonTerm("dummy", Helper::Type::BitVec64);
    auto b = new BVVariable(contexts, "b", dummyNonTerm);
    auto a = new BVVariable(contexts, "a", dummyNonTerm);
    auto d = new BVVariable(contexts, "d", dummyNonTerm);
    auto n1 = new BVXor(a, d, dummyNonTerm);
    auto n2 = new BVSub(b, n1, dummyNonTerm);
    auto n2p = new BVMul(b, n2, dummyNonTerm);
    auto n3 = new BVNot(b, dummyNonTerm);
    auto n4 = new BVAdd(a, d, dummyNonTerm);
    auto n5 = new BVOr(n3, n4, dummyNonTerm);
    auto n5p = new BVMul(b, n5, dummyNonTerm);
    auto n6 = new BVSub(n2p, n5p, dummyNonTerm);

    auto n8 = new BVMul(b, a, dummyNonTerm);
    auto n9 = new BVSub(n8, a, dummyNonTerm);
    auto all = new BVAnd(n6, n9, dummyNonTerm);

    std::cout << all->getTrace(expectedValues);
    ASSERT_EQ(all->getValues(), expectedValues);
}

TEST(HELPERSUITE, FloatingPointerErrorTest) {
    //  (bvsdiv (bvnot (bvnot #x0000000000000001)) #xffffffffffffffff)
    NonTerminal dummyNonTerm("dummy", Helper::Type::BitVec64);
    uint64_t a = 0x8000000000000000;
    uint64_t b = 0xffffffffffffffff;
    auto aNode = new BVLiteral(1, a, dummyNonTerm);
    auto bNode = new BVLiteral(1, b, dummyNonTerm);
    auto bvnot = new BVNot(aNode, dummyNonTerm);
    auto bvnotnot = new BVNot(bvnot, dummyNonTerm);
    //auto last = new BVSDiv(bvnotnot, bNode, dummyNonTerm);
    auto last = new BVSDiv(bvnotnot, bNode, dummyNonTerm);
    std::cout << Helper::variantToString(last->getValues().at(0)) << std::endl;
}



    //std::string val = "0+106 769-858-438";
    //std::string target =  "+106 (769) 858-438";
    ////std::string val = "0+106 769-";
    ////std::string target =  "+106 (769)";
    //std::set<char> alphabet;
    //for(char c : val) {
    //    alphabet.insert(c);
    //}
    //for(char c : target) {
    //    alphabet.insert(c);
    //}
    //LevenshteinAutomatonCreator creator(alphabet, 5, target);
    //mata::nfa::Nfa nfa = creator.createLevenshteinAutomaton();
    //mata::Word word;
    //for(char c : val) {
    //    word.push_back(c);
    //}
    //mata::nfa::StateSet stateSet = {0};
    //for(char c : val) {
    //    auto stateSet1 = nfa.post(stateSet, mata::nfa::EPSILON);
    //    //stateSet = {};
    //    for(auto state : stateSet1) {
    //        stateSet.insert(state);
    //    }
    //    stateSet = nfa.post(stateSet, c);
    //    std::cout << c << ": { ";
    //    for(auto state : stateSet) {
    //        for(auto [key, val] : creator.stateLookUpMap) {
    //            if(val == state) {
    //                std::cout << "(" << key.first << ", " << key.second << "), ";
    //            }
    //        }
    //    }
    //    std::cout << " }" << std::endl;
    //}
    //for(auto state : nfa.final) {
    //    for (auto [key, val]: creator.stateLookUpMap) {
    //        if (val == state) {
    //            std::cout << "(" << key.first << ", " << key.second << "), ";
    //        }
    //    }
    //}
    //std::cout << std::endl;

    //nfa = mata::nfa::remove_epsilon(nfa);
    //std::cout << nfa.is_in_lang(word) << std::endl;
    //exit(0);
