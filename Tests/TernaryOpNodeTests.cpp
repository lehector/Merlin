#include "gtest/gtest.h"
#include "../ast/LiteralNodes/StringLiteral.h"
#include "../ast/TernaryOpNodes/StringReplace.h"
#include "../ast/LiteralNodes/BoolLiteral.h"
#include "../ast/TernaryOpNodes/StringITE.h"
#include "../ast/LiteralNodes/IntLiteral.h"
#include "../ast/TernaryOpNodes/IntITE.h"
#include "../ast/TernaryOpNodes/Substring.h"
#include "../ast/TernaryOpNodes/IndexOf.h"
#include "../ast/LiteralNodes/BVLiteral.h"
#include "../ast/TernaryOpNodes/BVITE.h"

#define STRINGTERNARYOPNODESUITE StringTernaryOpNodeSuite
#define INTTERNARYOPNODESUITE IntTernaryOpNodeSuite
#define BVTERNARYOPNODESUITE BVTernaryOpNodeSuite

TEST(STRINGTERNARYOPNODESUITE, Replace) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    std::string s0 = "hello hola world";
    std::string s1 = "hola";
    std::string s2 = "howdy";

    StringLiteral* arg0 = new StringLiteral(1, s0, dummyNonTerm);
    StringLiteral* arg1 = new StringLiteral(1, s1, dummyNonTerm);
    StringLiteral* arg2 = new StringLiteral(1, s2, dummyNonTerm);

    StringReplace* stringReplace = new StringReplace(arg0, arg1, arg2, dummyNonTerm);
    std::string out = std::get<std::string>(stringReplace->getValues().at(0));
    EXPECT_EQ("hello howdy world", out);

}

TEST(STRINGTERNARYOPNODESUITE, StringITE) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool cond = false;
    std::string s1 = "hola";
    std::string s2 = "howdy";

    BoolLiteral* arg0 = new BoolLiteral(1, cond, dummyNonTerm);
    StringLiteral* arg1 = new StringLiteral(1, s1, dummyNonTerm);
    StringLiteral* arg2 = new StringLiteral(1, s2, dummyNonTerm);

    StringITE* ite = new StringITE(arg0, arg1, arg2, dummyNonTerm);
    std::string out = std::get<std::string>(ite->getValues().at(0));
    EXPECT_EQ("howdy", out);

}

TEST(INTTERNARYOPNODESUITE, IntITE) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool cond = false;
    int s1 = 42;
    int s2 = 13;

    BoolLiteral* arg0 = new BoolLiteral(1, cond, dummyNonTerm);
    IntLiteral* arg1 = new IntLiteral(1, s1, dummyNonTerm);
    IntLiteral* arg2 = new IntLiteral(1, s2, dummyNonTerm);

    IntITE* ite = new IntITE(arg0, arg1, arg2, dummyNonTerm);
    int out = std::get<int>(ite->getValues().at(0));
    EXPECT_EQ(13, out);

}

TEST(STRINGTERNARYOPNODESUITE, Substring) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    std::string str = "hello world";
    int s1 = 6;
    int s2 = 4;

    StringLiteral* arg0 = new StringLiteral(1, str, dummyNonTerm);
    IntLiteral* arg1 = new IntLiteral(1, s1, dummyNonTerm);
    IntLiteral* arg2 = new IntLiteral(1, s2, dummyNonTerm);

    Substring* substr = new Substring(arg0, arg1, arg2, dummyNonTerm);
    std::string out = std::get<std::string>(substr->getValues().at(0));
    EXPECT_EQ("worl", out);

}

TEST(INTTERNARYOPNODESUITE, IndexOf) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // normal
    std::string haystack = "hello world";
    std::string needle = "lo";
    int s2 = 0;

    StringLiteral* arg0 = new StringLiteral(1, haystack, dummyNonTerm);
    StringLiteral* arg1 = new StringLiteral(1, needle, dummyNonTerm);
    IntLiteral* arg2 = new IntLiteral(1, s2, dummyNonTerm);

    IndexOf* indexof = new IndexOf(arg0, arg1, arg2, dummyNonTerm);
    int out = std::get<int>(indexof->getValues().at(0));
    EXPECT_EQ(3, out);



    // skip one occurence
    haystack = "hello world";
    needle = "l";
    s2 = 5;

    arg0 = new StringLiteral(1, haystack, dummyNonTerm);
    arg1 = new StringLiteral(1, needle, dummyNonTerm);
    arg2 = new IntLiteral(1, s2, dummyNonTerm);

    indexof = new IndexOf(arg0, arg1, arg2, dummyNonTerm);
    out = std::get<int>(indexof->getValues().at(0));
    EXPECT_EQ(9, out);


    // no occurence
    haystack = "hello world";
    needle = "x";
    s2 = 5;

    arg0 = new StringLiteral(1, haystack, dummyNonTerm);
    arg1 = new StringLiteral(1, needle, dummyNonTerm);
    arg2 = new IntLiteral(1, s2, dummyNonTerm);

    indexof = new IndexOf(arg0, arg1, arg2, dummyNonTerm);
    out = std::get<int>(indexof->getValues().at(0));
    EXPECT_EQ(-1, out);


    // out of range
    haystack = "hello world";
    needle = "l";
    s2 = 12;

    arg0 = new StringLiteral(1, haystack, dummyNonTerm);
    arg1 = new StringLiteral(1, needle, dummyNonTerm);
    arg2 = new IntLiteral(1, s2, dummyNonTerm);

    indexof = new IndexOf(arg0, arg1, arg2, dummyNonTerm);
    out = std::get<int>(indexof->getValues().at(0));
    EXPECT_EQ(-1, out);

}

TEST(BVTERNARYOPNODESUITE, BVITE) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool cond = false;
    uint64_t s1 = (uint64_t) 42;
    uint64_t s2 = (uint64_t) 13;

    BoolLiteral* arg0 = new BoolLiteral(1, cond, dummyNonTerm);
    BVLiteral* arg1 = new BVLiteral(1, s1, dummyNonTerm);
    BVLiteral* arg2 = new BVLiteral(1, s2, dummyNonTerm);

    BVITE* ite = new BVITE(arg0, arg1, arg2, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(ite->getValues().at(0));
    EXPECT_EQ((uint64_t) 13, out);

}