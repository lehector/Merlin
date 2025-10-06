#include "gtest/gtest.h"
#include "../ast/LiteralNodes/IntLiteral.h"
#include "../ast/UnaryOpNodes/IntToString.h"
#include "../ast/LiteralNodes/StringLiteral.h"
#include "../ast/UnaryOpNodes/StringToInt.h"
#include "../ast/UnaryOpNodes/StringLength.h"
#include "../ast/LiteralNodes/BVLiteral.h"
#include "../ast/UnaryOpNodes/BVNot.h"
#include "../ast/UnaryOpNodes/BVNeg.h"
#include "../ast/LiteralNodes/BoolLiteral.h"
#include "../ast/UnaryOpNodes/LNot.h"
#include "../ast/UnaryOpNodes/BVRedor.h"


#define BVUNARYOPNODESUITE BVUnaryOpNodeSuite
#define INTUNARYOPNODESUITE IntUnaryOpNodeSuite
#define STRINGUNARYOPNODESUITE StringUnaryOpNodeSuite
#define BOOLUNARYOPNODESUITE BoolUnaryOpNodeSuite


TEST(STRINGUNARYOPNODESUITE, IntToString) {
    // natural number
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    int s0 = 2;

    IntLiteral* arg0 = new IntLiteral(1, s0, dummyNonTerm);

    IntToString* intToString = new IntToString(arg0, dummyNonTerm);
    std::string out = std::get<std::string>(intToString->getValues().at(0));
    EXPECT_EQ("2", out);

    // non-natural number
    s0 = -2;

    arg0 = new IntLiteral(1, s0, dummyNonTerm);

    intToString = new IntToString(arg0, dummyNonTerm);
    out = std::get<std::string>(intToString->getValues().at(0));
    EXPECT_EQ("", out);

}

TEST(INTUNARYOPNODESUITE, StringToInt) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // natural number
    std::string s0 = "123";

    StringLiteral* arg0 = new StringLiteral(1, s0, dummyNonTerm);

    StringToInt* stringToInt = new StringToInt(arg0, dummyNonTerm);
    int out = std::get<int>(stringToInt->getValues().at(0));
    EXPECT_EQ(123, out);

    // non-natural number
    s0 = "-123";

    arg0 = new StringLiteral(1, s0, dummyNonTerm);

    stringToInt = new StringToInt(arg0, dummyNonTerm);
    out = std::get<int>(stringToInt->getValues().at(0));
    EXPECT_EQ(-1, out);

    // not a number
    s0 = "abc";

    arg0 = new StringLiteral(1, s0, dummyNonTerm);

    stringToInt = new StringToInt(arg0, dummyNonTerm);
    out = std::get<int>(stringToInt->getValues().at(0));
    EXPECT_EQ(-1, out);

}

TEST(INTUNARYOPNODESUITE, StringLength) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // natural number
    std::string s0 = "123";

    StringLiteral* arg0 = new StringLiteral(1, s0, dummyNonTerm);

    StringLength* strlen = new StringLength(arg0, dummyNonTerm);
    int out = std::get<int>(strlen->getValues().at(0));
    EXPECT_EQ(3, out);

}

TEST(BVUNARYOPNODESUITE, BVNot) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t s0 = 0b101;

    BVLiteral* arg0 = new BVLiteral(1, s0, dummyNonTerm);

    BVNot* bvnot = new BVNot(arg0, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvnot->getValues().at(0));
    uint64_t exp = 0b1111111111111111111111111111111111111111111111111111111111111010;
    EXPECT_EQ(exp, out);

}

TEST(BVUNARYOPNODESUITE, BVNeg) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t s0 = 2;

    BVLiteral* arg0 = new BVLiteral(1, s0, dummyNonTerm);

    BVNeg* bvneg = new BVNeg(arg0, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvneg->getValues().at(0));
    EXPECT_EQ((uint64_t) -2, out);

}

TEST(BOOLUNARYOPNODESUITE, LNot) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool s0 = false;

    BoolLiteral* arg0 = new BoolLiteral(1, s0, dummyNonTerm);

    LNot* lnot = new LNot(arg0, dummyNonTerm);
    bool out = std::get<bool>(lnot->getValues().at(0));
    EXPECT_EQ(true, out);

}

TEST(BOOLUNARYOPNODESUITE, BVRedor) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t s0 = 0b000100;

    BVLiteral* arg0 = new BVLiteral(1, s0, dummyNonTerm);

    BVRedor* bvredor = new BVRedor(arg0, dummyNonTerm);
    bool out = std::get<bool>(bvredor->getValues().at(0));
    EXPECT_EQ(true, out);


    s0 = 0b00000;

    arg0 = new BVLiteral(1, s0, dummyNonTerm);

    bvredor = new BVRedor(arg0, dummyNonTerm);
    out = std::get<bool>(bvredor->getValues().at(0));
    EXPECT_EQ(false, out);

}