#include "gtest/gtest.h"
#include "../ast/LiteralNodes/IntLiteral.h"
#include "../ast/BinaryOpNodes/IntAddition.h"
#include "../ast/LiteralNodes/StringLiteral.h"
#include "../ast/BinaryOpNodes/StringConcat.h"
#include "../ast/BinaryOpNodes/StringAt.h"
#include "../ast/BinaryOpNodes/IntSubstraction.h"
#include "../ast/BinaryOpNodes/IntLessThanEq.h"
#include "../ast/LiteralNodes/BoolLiteral.h"
#include "../ast/BinaryOpNodes/BoolEquals.h"
#include "../ast/BinaryOpNodes/IntEquals.h"
#include "../ast/BinaryOpNodes/PrefixOf.h"
#include "../ast/BinaryOpNodes/SuffixOf.h"
#include "../ast/BinaryOpNodes/Contains.h"
#include "../ast/LiteralNodes/BVLiteral.h"
#include "../ast/BinaryOpNodes/BVAnd.h"
#include "../ast/BinaryOpNodes/BVOr.h"
#include "../ast/BinaryOpNodes/BVXor.h"
#include "../ast/BinaryOpNodes/BVShiftLeft.h"
#include "../ast/BinaryOpNodes/BVAdd.h"
#include "../ast/BinaryOpNodes/BVSub.h"
#include "../ast/BinaryOpNodes/BVSDiv.h"
#include "../ast/BinaryOpNodes/BVUDiv.h"
#include "../ast/BinaryOpNodes/BVMul.h"
#include "../ast/BinaryOpNodes/BVShrLogical.h"
#include "../ast/BinaryOpNodes/BVShrArithmetic.h"
#include "../ast/BinaryOpNodes/BVEquals.h"
#include "../ast/BinaryOpNodes/BVSlt.h"
#include "../ast/BinaryOpNodes/BVSle.h"
#include "../ast/BinaryOpNodes/BVUlt.h"
#include "../ast/BinaryOpNodes/BVUle.h"
#include "../ast/BinaryOpNodes/BVUgt.h"
#include "../ast/BinaryOpNodes/LAnd.h"
#include "../ast/BinaryOpNodes/LOr.h"
#include "../ast/BinaryOpNodes/LXor.h"
#include "../ast/BinaryOpNodes/BVSRem.h"
#include "../ast/BinaryOpNodes/BVURem.h"
#include "../ast/UnaryOpNodes/BVNot.h"
#include "../ast/UnaryOpNodes/BVNeg.h"

#define STRINGBINARYOPNODESUITE StringBinaryOpNodeSuite
#define INTBINARYOPNODESUITE IntBinaryOpNodeSuite
#define BOOLBINARYOPNODESUITE BoolBinaryOpNodeSuite
#define BVBINARYOPNODESUITE BVBinaryOpNodeSuite

TEST(INTBINARYOPNODESUITE, Addition) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    int valL = 42;
    int valR = 13;
    IntLiteral* l = new IntLiteral(1, valL, dummyNonTerm);
    IntLiteral* r = new IntLiteral(1, valR, dummyNonTerm);
    IntAddition* addition = new IntAddition(l, r, dummyNonTerm);
    int out = std::get<int>(addition->getValues().at(0));
    EXPECT_EQ(out, valL + valR);
}

TEST(INTBINARYOPNODESUITE, Substraction) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    int valL = 42;
    int valR = 13;
    IntLiteral* l = new IntLiteral(1, valL, dummyNonTerm);
    IntLiteral* r = new IntLiteral(1, valR, dummyNonTerm);
    IntSubstraction* substraction = new IntSubstraction(l, r, dummyNonTerm);
    int out = std::get<int>(substraction->getValues().at(0));
    EXPECT_EQ(out, valL - valR);
}

TEST(BOOLBINARYOPNODESUITE, IntLessThanEq) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // true
    int valL = 0;
    int valR = 13;
    IntLiteral* l = new IntLiteral(1, valL, dummyNonTerm);
    IntLiteral* r = new IntLiteral(1, valR, dummyNonTerm);
    IntLessThanEq* lessThanEq = new IntLessThanEq(l, r, dummyNonTerm);
    bool out = std::get<bool>(lessThanEq->getValues().at(0));
    EXPECT_EQ(out, true);

    //false
    valL = 42;
    valR = 13;
    l = new IntLiteral(1, valL, dummyNonTerm);
    r = new IntLiteral(1, valR, dummyNonTerm);
    lessThanEq = new IntLessThanEq(l, r, dummyNonTerm);
    out = std::get<bool>(lessThanEq->getValues().at(0));
    EXPECT_EQ(out, false);
}

TEST(BOOLBINARYOPNODESUITE, BoolEq) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // true
    bool valL = false;
    bool valR = false;
    BoolLiteral* l = new BoolLiteral(1, valL, dummyNonTerm);
    BoolLiteral* r = new BoolLiteral(1, valR, dummyNonTerm);
    BoolEquals* eq = new BoolEquals(l, r, dummyNonTerm);
    bool out = std::get<bool>(eq->getValues().at(0));
    EXPECT_EQ(out, true);

    // false
    valL = false;
    valR = true;
    l = new BoolLiteral(1, valL, dummyNonTerm);
    r = new BoolLiteral(1, valR, dummyNonTerm);
    eq = new BoolEquals(l, r, dummyNonTerm);
    out = std::get<bool>(eq->getValues().at(0));
    EXPECT_EQ(out, false);
}

TEST(BOOLBINARYOPNODESUITE, IntEq) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // false
    int valL = 1;
    int valR = 2;
    IntLiteral* l = new IntLiteral(1, valL, dummyNonTerm);
    IntLiteral* r = new IntLiteral(1, valR, dummyNonTerm);
    IntEquals* eq = new IntEquals(l, r, dummyNonTerm);
    bool out = std::get<bool>(eq->getValues().at(0));
    EXPECT_EQ(out, false);

    // true
    valL = 2;
    valR = 2;
    l = new IntLiteral(1, valL, dummyNonTerm);
    r = new IntLiteral(1, valR, dummyNonTerm);
    eq = new IntEquals(l, r, dummyNonTerm);
    out = std::get<bool>(eq->getValues().at(0));
    EXPECT_EQ(out, true);
}

TEST(BOOLBINARYOPNODESUITE, PrefixOf) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // false
    std::string valL = "world";
    std::string valR = "hello world";
    StringLiteral* l = new StringLiteral(1, valL, dummyNonTerm);
    StringLiteral* r = new StringLiteral(1, valR, dummyNonTerm);
    PrefixOf* prefix = new PrefixOf(l, r, dummyNonTerm);
    bool out = std::get<bool>(prefix->getValues().at(0));
    EXPECT_EQ(out, false);

    // true
    valL = "hello";
    valR = "hello world";
    l = new StringLiteral(1, valL, dummyNonTerm);
    r = new StringLiteral(1, valR, dummyNonTerm);
    prefix = new PrefixOf(l, r, dummyNonTerm);
    out = std::get<bool>(prefix->getValues().at(0));
    EXPECT_EQ(out, true);
}

TEST(BOOLBINARYOPNODESUITE, SuffixOf) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // false
    std::string valL = "hello";
    std::string valR = "hello world";
    StringLiteral* l = new StringLiteral(1, valL, dummyNonTerm);
    StringLiteral* r = new StringLiteral(1, valR, dummyNonTerm);
    SuffixOf* suffix = new SuffixOf(l, r, dummyNonTerm);
    bool out = std::get<bool>(suffix->getValues().at(0));
    EXPECT_EQ(out, false);

    // true
    valL = "world";
    valR = "hello world";
    l = new StringLiteral(1, valL, dummyNonTerm);
    r = new StringLiteral(1, valR, dummyNonTerm);
    suffix = new SuffixOf(l, r, dummyNonTerm);
    out = std::get<bool>(suffix->getValues().at(0));
    EXPECT_EQ(out, true);
}

TEST(BOOLBINARYOPNODESUITE, Contains) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // false
    std::string valL = "hello world";
    std::string valR = "jakob";
    StringLiteral* l = new StringLiteral(1, valL, dummyNonTerm);
    StringLiteral* r = new StringLiteral(1, valR, dummyNonTerm);
    Contains* contains = new Contains(l, r, dummyNonTerm);
    bool out = std::get<bool>(contains->getValues().at(0));
    EXPECT_EQ(out, false);

    // true
    valL = "hello world";
    valR = "ello wor";
    l = new StringLiteral(1, valL, dummyNonTerm);
    r = new StringLiteral(1, valR, dummyNonTerm);
    contains = new Contains(l, r, dummyNonTerm);
    out = std::get<bool>(contains->getValues().at(0));
    EXPECT_EQ(out, true);
}


TEST(STRINGBINARYOPNODESUITE, includesVar) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    std::string valL = "hello";
    std::string valR = "world";
    StringLiteral* l = new StringLiteral(1, valL, dummyNonTerm);
    StringLiteral* r = new StringLiteral(1, valR, dummyNonTerm);
    StringConcat* concat = new StringConcat(l, r, dummyNonTerm);
    bool out = concat->includes("myvar");
    EXPECT_EQ(out, false);
}

TEST(STRINGBINARYOPNODESUITE, Concat) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    std::string valL = "hello";
    std::string valR = "world";
    StringLiteral* l = new StringLiteral(1, valL, dummyNonTerm);
    StringLiteral* r = new StringLiteral(1, valR, dummyNonTerm);
    StringConcat* concat = new StringConcat(l, r, dummyNonTerm);
    std::string out = std::get<std::string>(concat->getValues().at(0));
    EXPECT_EQ(out, "helloworld");
}

TEST(STRINGBINARYOPNODESUITE, At) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    // normal
    std::string valL = "hello";
    int valR = 1;
    StringLiteral* l = new StringLiteral(1, valL, dummyNonTerm);
    IntLiteral* r = new IntLiteral(1, valR, dummyNonTerm);
    StringAt* at = new StringAt(l, r, dummyNonTerm);
    std::string out = std::get<std::string>(at->getValues().at(0));
    EXPECT_EQ(out, "e");

    // too small
    valR = -10;
    l = new StringLiteral(1, valL, dummyNonTerm);
    r = new IntLiteral(1, valR, dummyNonTerm);
    at = new StringAt(l, r, dummyNonTerm);
    out = std::get<std::string>(at->getValues().at(0));
    EXPECT_EQ(out, "");

    // too big
    valR = valL.length();
    l = new StringLiteral(1, valL, dummyNonTerm);
    r = new IntLiteral(1, valR, dummyNonTerm);
    at = new StringAt(l, r, dummyNonTerm);
    out = std::get<std::string>(at->getValues().at(0));
    EXPECT_EQ(out, "");
}

TEST(BVBINARYOPNODESUITE, BVAnd) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = 0b011;
    uint64_t valR = 0b110;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVAnd* bvand = new BVAnd(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvand->getValues().at(0));
    EXPECT_EQ(out, 0b010);
}

TEST(BVBINARYOPNODESUITE, BVOr) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = 0b001;
    uint64_t valR = 0b100;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVOr* bvor = new BVOr(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvor->getValues().at(0));
    EXPECT_EQ(out, 0b101);
}

TEST(BVBINARYOPNODESUITE, BVXor) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = 0b011;
    uint64_t valR = 0b110;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVXor* bvxor = new BVXor(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvxor->getValues().at(0));
    EXPECT_EQ(out, 0b101);
}

TEST(BVBINARYOPNODESUITE, BVShiftLeft) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = 0b001100;
    uint64_t valR = 1;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVShiftLeft* bvshl = new BVShiftLeft(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvshl->getValues().at(0));
    EXPECT_EQ(out, 0b011000);

    valL = 0b001100;
    valR = 65;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvshl = new BVShiftLeft(l, r, dummyNonTerm);
    out = std::get<uint64_t>(bvshl->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) 0);
}

TEST(BVBINARYOPNODESUITE, BVAdd) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = 0b001100;
    uint64_t valR = 0b101000;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVAdd* bvadd = new BVAdd(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvadd->getValues().at(0));
    EXPECT_EQ(out, 0b110100);
}

TEST(BVBINARYOPNODESUITE, BVSub) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = 0b101100;
    uint64_t valR = 0b001000;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVSub* bvsub = new BVSub(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvsub->getValues().at(0));
    EXPECT_EQ(out, 0b100100);
}

TEST(BVBINARYOPNODESUITE, BVSDiv) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) -26;
    uint64_t valR = (uint64_t) 2;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVSDiv* bvsdiv = new BVSDiv(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvsdiv->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) -13);

    // divide by zero returns zero
    valL = (uint64_t) 26;
    valR = (uint64_t) 0;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvsdiv = new BVSDiv(l, r, dummyNonTerm);
    out = std::get<uint64_t>(bvsdiv->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) 0xffffffffffffffff);
    //EXPECT_EQ(out, (uint64_t) 0);
}

TEST(BVBINARYOPNODESUITE, BVUDiv) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 26;
    uint64_t valR = (uint64_t) 2;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVUDiv* bvudiv = new BVUDiv(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvudiv->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) 13);

    // divide by zero returns 0xffffffffffffffff
    valL = (uint64_t) 26;
    valR = (uint64_t) 0;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvudiv = new BVUDiv(l, r, dummyNonTerm);
    out = std::get<uint64_t>(bvudiv->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) 0xffffffffffffffff);


    // behaves differently on "signed" values
    valL = (uint64_t) -26;
    valR = (uint64_t) 2;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvudiv = new BVUDiv(l, r, dummyNonTerm);
    out = std::get<uint64_t>(bvudiv->getValues().at(0));
    EXPECT_NE(out, (uint64_t) -13);
}

TEST(BVBINARYOPNODESUITE, BVMul) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 10;
    uint64_t valR = (uint64_t) 2;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVMul* bvmul = new BVMul(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvmul->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) 20);
}

TEST(BVBINARYOPNODESUITE, BVShrLogical) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 0b010000;
    uint64_t valR = (uint64_t) 2;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVShrLogical* bvlshr = new BVShrLogical(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvlshr->getValues().at(0));
    EXPECT_EQ(out, (uint64_t) 0b000100);
}

TEST(BVBINARYOPNODESUITE, BVShrArithmetic) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 0b1000000000000000000000000000000000000000000000000000000000000000; // 1.0^63
    uint64_t valR = (uint64_t) 2;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVShrArithmetic* bvashr = new BVShrArithmetic(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvashr->getValues().at(0));
    uint64_t exp = (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000; // 111.0^61
    EXPECT_EQ(out, exp);
}

TEST(BOOLBINARYOPNODESUITE, BVEquals) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 0b101010;
    uint64_t valR = (uint64_t) 0b101010;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVEquals* bveq = new BVEquals(l, r, dummyNonTerm);
    bool out = std::get<bool>(bveq->getValues().at(0));
    EXPECT_TRUE(out);

    valL = (uint64_t) 0b101010;
    valR = (uint64_t) 0b101011;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bveq = new BVEquals(l, r, dummyNonTerm);
    out = std::get<bool>(bveq->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, BVSlt) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) -1;
    uint64_t valR = (uint64_t) 1;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVSlt* bvslt = new BVSlt(l, r, dummyNonTerm);
    bool out = std::get<bool>(bvslt->getValues().at(0));
    EXPECT_TRUE(out);

    valL = (uint64_t) 1;
    valR = (uint64_t) -1;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvslt = new BVSlt(l, r, dummyNonTerm);
    out = std::get<bool>(bvslt->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, BVSle) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) -1;
    uint64_t valR = (uint64_t) -1;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVSle* bvsle = new BVSle(l, r, dummyNonTerm);
    bool out = std::get<bool>(bvsle->getValues().at(0));
    EXPECT_TRUE(out);

    valL = (uint64_t) 1;
    valR = (uint64_t) -1;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvsle = new BVSle(l, r, dummyNonTerm);
    out = std::get<bool>(bvsle->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, BVUlt) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 1;
    uint64_t valR = (uint64_t) -1; // is greater in unsigned interpretation
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVUlt* bvult = new BVUlt(l, r, dummyNonTerm);
    bool out = std::get<bool>(bvult->getValues().at(0));
    EXPECT_TRUE(out);

    valL = (uint64_t) 1;
    valR = (uint64_t) 0;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvult = new BVUlt(l, r, dummyNonTerm);
    out = std::get<bool>(bvult->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, BVUle) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 1;
    uint64_t valR = (uint64_t) 1;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVUle* bvule = new BVUle(l, r, dummyNonTerm);
    bool out = std::get<bool>(bvule->getValues().at(0));
    EXPECT_TRUE(out);

    valL = (uint64_t) -1; // is greater in unsigned interpretation
    valR = (uint64_t) 0;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvule = new BVUle(l, r, dummyNonTerm);
    out = std::get<bool>(bvule->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, BVUgt) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) -1; // is greater in unsigned interp
    uint64_t valR = (uint64_t) 1;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVUgt* bvugt = new BVUgt(l, r, dummyNonTerm);
    bool out = std::get<bool>(bvugt->getValues().at(0));
    EXPECT_TRUE(out);

    valL = (uint64_t) 0;
    valR = (uint64_t) 1;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvugt = new BVUgt(l, r, dummyNonTerm);
    out = std::get<bool>(bvugt->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, LAnd) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool valL = true;
    bool valR = true;
    BoolLiteral* l = new BoolLiteral(1, valL, dummyNonTerm);
    BoolLiteral* r = new BoolLiteral(1, valR, dummyNonTerm);
    LAnd* land = new LAnd(l, r, dummyNonTerm);
    bool out = std::get<bool>(land->getValues().at(0));
    EXPECT_TRUE(out);

    valL = true;
    valR = false;
    l = new BoolLiteral(1, valL, dummyNonTerm);
    r = new BoolLiteral(1, valR, dummyNonTerm);
    land = new LAnd(l, r, dummyNonTerm);
    out = std::get<bool>(land->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, LOr) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool valL = false;
    bool valR = true;
    BoolLiteral* l = new BoolLiteral(1, valL, dummyNonTerm);
    BoolLiteral* r = new BoolLiteral(1, valR, dummyNonTerm);
    LOr* lor = new LOr(l, r, dummyNonTerm);
    bool out = std::get<bool>(lor->getValues().at(0));
    EXPECT_TRUE(out);

    valL = false;
    valR = false;
    l = new BoolLiteral(1, valL, dummyNonTerm);
    r = new BoolLiteral(1, valR, dummyNonTerm);
    lor = new LOr(l, r, dummyNonTerm);
    out = std::get<bool>(lor->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BOOLBINARYOPNODESUITE, LXor) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    bool valL = false;
    bool valR = true;
    BoolLiteral* l = new BoolLiteral(1, valL, dummyNonTerm);
    BoolLiteral* r = new BoolLiteral(1, valR, dummyNonTerm);
    LXor* lxor = new LXor(l, r, dummyNonTerm);
    bool out = std::get<bool>(lxor->getValues().at(0));
    EXPECT_TRUE(out);

    valL = true;
    valR = true;
    l = new BoolLiteral(1, valL, dummyNonTerm);
    r = new BoolLiteral(1, valR, dummyNonTerm);
    lxor = new LXor(l, r, dummyNonTerm);
    out = std::get<bool>(lxor->getValues().at(0));
    EXPECT_FALSE(out);
}

TEST(BVBINARYOPNODESUITE, BVSRem) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) -10;
    uint64_t valR = (uint64_t) 3;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVSRem* bvsrem = new BVSRem(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvsrem->getValues().at(0));
    uint64_t exp = (uint64_t) -1; // -10 / 3 == -3; *td::shared_ptr<3 > -3 == -9; (-9) + (-1) = (-10)
    EXPECT_EQ(out, exp);
}

TEST(BVBINARYOPNODESUITE, BVURem) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valL = (uint64_t) 10;
    uint64_t valR = (uint64_t) 3;
    BVLiteral* l = new BVLiteral(1, valL, dummyNonTerm);
    BVLiteral* r = new BVLiteral(1, valR, dummyNonTerm);
    BVURem* bvurem = new BVURem(l, r, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvurem->getValues().at(0));
    uint64_t exp = (uint64_t) 1;
    EXPECT_EQ(out, exp);

    valL = (uint64_t) 0xffffffffffffffff;
    valR = (uint64_t) 0x0000000000000000;
    l = new BVLiteral(1, valL, dummyNonTerm);
    r = new BVLiteral(1, valR, dummyNonTerm);
    bvurem = new BVURem(l, r, dummyNonTerm);
    out = std::get<uint64_t>(bvurem->getValues().at(0));
    EXPECT_EQ((~out) < valL, true);

}

TEST(BVBINARYOPNODESUITE, BVShrArithmetic2) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valX = (uint64_t) 0xfffffffe00000001;
    uint64_t valM = (uint64_t) 0xffffffffffffffff;
    uint64_t valConst = (uint64_t) 0x000000000000001f;
    auto l1 = new BVLiteral(1, valM, dummyNonTerm);
    auto r1 = new BVLiteral(1, valConst, dummyNonTerm);
    auto bvsub = new BVSub(l1, r1, dummyNonTerm);
    auto l = new BVLiteral(1, valX, dummyNonTerm);
    BVShrArithmetic* bvashr = new BVShrArithmetic(l, bvsub, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvashr->getValues().at(0));
    uint64_t exp = (uint64_t) 0xffffffffffffffff;
    EXPECT_EQ(out, exp);
}

TEST(BVBINARYOPNODESUITE, BVSub2) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valM = (uint64_t) 0xffffffffffffffff;
    uint64_t valConst = (uint64_t) 0x000000000000001f;
    auto l1 = new BVLiteral(1, valM, dummyNonTerm);
    auto r1 = new BVLiteral(1, valConst, dummyNonTerm);
    auto bvsub = new BVSub(l1, r1, dummyNonTerm);
    uint64_t out = std::get<uint64_t>(bvsub->getValues().at(0));
    uint64_t exp = (uint64_t) 0xffffffffffffffe0;
    EXPECT_EQ(out, exp);
}

//(bvult (bvnot (bvurem x y)) x)
TEST(BVBINARYOPNODESUITE, BVUlt2) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valX = (uint64_t) 0xffffffffffffffff;
    uint64_t valY = (uint64_t) 0x0000000000000000;
    auto x = new BVLiteral(1, valX, dummyNonTerm);
    auto y = new BVLiteral(1, valY, dummyNonTerm);
    auto bvUrem = new BVURem(x, y, dummyNonTerm);
    auto bvNot = new BVNot(bvUrem, dummyNonTerm);
    auto bvUlt = new BVUlt(bvNot, x, dummyNonTerm);
    bool out = std::get<bool>(bvUlt->getValues().at(0));
    bool exp = true;
    EXPECT_EQ(out, exp);
}

//(bvnot (bvshl (bvurem #x000000000000001f (bvand (bvneg x) (bvnot x))) #x0000000000000001))
TEST(BVBINARYOPNODESUITE, BVError) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valX = (uint64_t) 0xffffc00000000000;
    auto x = new BVLiteral(1, valX, dummyNonTerm);
    auto valConst1 = 0x0000000000000001;
    auto valConst2 = 0x000000000000001f;
    auto const1 = new BVLiteral(1, valConst1, dummyNonTerm);
    auto const2 = new BVLiteral(1, valConst2, dummyNonTerm);
    auto bvnegx = new BVNeg(x, dummyNonTerm);
    auto bvnotx = new BVNot(x, dummyNonTerm);
    auto bvand = new BVAnd(bvnegx, bvnotx, dummyNonTerm);
    auto bvurem = new BVURem(const2, bvand, dummyNonTerm);
    auto bvshl = new BVShiftLeft(bvurem, const1, dummyNonTerm);
    auto bvnot = new BVNot(bvshl, dummyNonTerm);
    auto out = std::get<uint64_t>(bvnot->getValues().at(0));
    uint64_t exp = 0b1111111111111111111111111111111111111111111111111111111111000001;
    EXPECT_EQ(out, exp);
}

//(bvshl (bvurem #x000000000000001f (bvand (bvneg x) (bvnot x))) #x0000000000000001)
TEST(BVBINARYOPNODESUITE, BVError1) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valX = (uint64_t) 0xffffc00000000000;
    auto x = new BVLiteral(1, valX, dummyNonTerm);
    auto valConst1 = 0x0000000000000001;
    auto valConst2 = 0x000000000000001f;
    auto const1 = new BVLiteral(1, valConst1, dummyNonTerm);
    auto const2 = new BVLiteral(1, valConst2, dummyNonTerm);
    auto bvnegx = new BVNeg(x, dummyNonTerm);
    auto bvnotx = new BVNot(x, dummyNonTerm);
    auto bvand = new BVAnd(bvnegx, bvnotx, dummyNonTerm);
    auto bvurem = new BVURem(const2, bvand, dummyNonTerm);
    auto bvshl = new BVShiftLeft(bvurem, const1, dummyNonTerm);
    auto out = std::get<uint64_t>(bvshl->getValues().at(0));
    uint64_t exp = 0b0000000000000000000000000000000000000000000000000000000000111110;
    EXPECT_EQ(out, exp);
}

//(bvurem #x000000000000001f (bvand (bvneg x) (bvnot x)))
TEST(BVBINARYOPNODESUITE, BVError2) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valX = (uint64_t) 0xffffc00000000000;
    auto x = new BVLiteral(1, valX, dummyNonTerm);
    auto valConst1 = 0x0000000000000001;
    auto valConst2 = 0x000000000000001f;
    auto const1 = new BVLiteral(1, valConst1, dummyNonTerm);
    auto const2 = new BVLiteral(1, valConst2, dummyNonTerm);
    auto bvnegx = new BVNeg(x, dummyNonTerm);
    auto bvnotx = new BVNot(x, dummyNonTerm);
    auto bvand = new BVAnd(bvnegx, bvnotx, dummyNonTerm);
    auto bvurem = new BVURem(const2, bvand, dummyNonTerm);
    auto out = std::get<uint64_t>(bvurem->getValues().at(0));
    uint64_t exp = 0b0000000000000000000000000000000000000000000000000000000000011111;
    EXPECT_EQ(out, exp);
}

//(bvand (bvneg x) (bvnot x))
TEST(BVBINARYOPNODESUITE, BVError3) {
    NonTerminal dummyNonTerm("dummy", Helper::Int);
    uint64_t valX = (uint64_t) 0xffffc00000000000;
    auto x = new BVLiteral(1, valX, dummyNonTerm);
    auto valConst1 = 0x0000000000000001;
    auto valConst2 = 0x000000000000001f;
    auto const1 = new BVLiteral(1, valConst1, dummyNonTerm);
    auto const2 = new BVLiteral(1, valConst2, dummyNonTerm);
    auto bvnegx = new BVNeg(x, dummyNonTerm);
    auto bvnotx = new BVNot(x, dummyNonTerm);
    auto bvand = new BVAnd(bvnegx, bvnotx, dummyNonTerm);
    auto out = std::get<uint64_t>(bvand->getValues().at(0));
    uint64_t exp = 0b0000000000000000000000000000000000000000000000000000000000000000;
    EXPECT_EQ(out, exp);
}