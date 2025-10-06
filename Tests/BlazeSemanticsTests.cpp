#include "../ast/LiteralNode.h"
#include "../ast/LiteralNodes/StringLiteral.h"
#include "../ast/BinaryOpNodes/StringConcat.h"
#include "../ast/Blaze/ConstStr.h"
#include "../ast/Blaze/Pos.h"
#include "../ast/Blaze/Str.h"
#include "../ast/Blaze/Substr.h"
#include "../ast/LiteralNodes/IntLiteral.h"
#include "gtest/gtest.h"

TEST(BLAZESUITE, nameCombine) {
    std::string input = "Launa|Withers";
    std::string expectedOutput = "Withers, L.";
    // SubStr(x, Pos(x, 'Sep', 1, 1), Pos(x, 'ProperCase', -1, 1)
    NonTerminal dummyNonTerm("dummy", Helper::String);
    NonTerminal tokenNonTerm("ntToken", Helper::String);
    NonTerminal ntInt("ntInt", Helper::Int);
    NonTerminal ntDir("ntDir", Helper::Int);

    auto x = new LiteralNode<std::string>(1, input, dummyNonTerm);
    auto sep = new StringLiteral(1, "Sep", tokenNonTerm);
    auto oneInt = new IntLiteral(1, 1, ntInt);
    auto oneDir = new IntLiteral(1, 1, ntDir);
    auto pos1 = new Pos(x, sep, oneInt, oneDir, dummyNonTerm);
    auto pos1Val = std::get<int>(pos1->getValues().at(0));
    EXPECT_EQ(pos1Val, 6);

    auto propercase = new StringLiteral(1, "ProperCase", tokenNonTerm);
    auto minusOne = new IntLiteral(1, -1, ntInt);
    auto pos2 = new Pos(x, propercase, minusOne, oneDir, dummyNonTerm);
    auto pos2Val = std::get<int>(pos2->getValues().at(0));
    EXPECT_EQ(pos2Val, 13);

    auto substr1 = new Substr(x, pos1, pos2, dummyNonTerm);
    auto substr1Val = std::get<std::string>(substr1->getValues().at(0));
    EXPECT_EQ(substr1Val, "Withers");

    auto commaLiteral = new StringLiteral(1, ",", dummyNonTerm);
    auto spaceLiteral = new StringLiteral(1, " ", dummyNonTerm);
    auto dotLiteral = new StringLiteral(1, ".", dummyNonTerm);

    auto comma = new ConstStr(commaLiteral, dummyNonTerm);
    auto space = new ConstStr(spaceLiteral, dummyNonTerm);
    auto dotConstStr = new ConstStr(dotLiteral, dummyNonTerm);
    auto dot = new Str(dotConstStr, dummyNonTerm);

    auto zero = new IntLiteral(1, 0, ntDir);

    auto caps = new StringLiteral(1, "CAPS", tokenNonTerm);

    // SubStr(x, Pos(x, 'CAPS', 1, 0), Pos(x, 'CAPS', 1, 1))
    auto pos3 = new Pos(x, caps, oneInt, zero, dummyNonTerm);
    auto pos4 = new Pos(x, caps, oneInt, oneDir, dummyNonTerm);
    auto substr2 = new Substr(x, pos3, pos4, dummyNonTerm);
    auto substr2Val = std::get<std::string>(substr2->getValues().at(0));
    EXPECT_EQ(substr2Val, "L");


    NonTerminal concatNonTerm("concat", Helper::String);
    auto concat1 = new StringConcat(substr2, dot, concatNonTerm);
    auto concat2 = new StringConcat(space, concat1, concatNonTerm);
    auto concat3 = new StringConcat(comma, concat2, concatNonTerm);
    auto concat4 = new StringConcat(substr1, concat3, concatNonTerm);
    auto concat4Val = std::get<std::string>(concat4->getValues().at(0));

    EXPECT_EQ(concat4->getCost(), 76);

    EXPECT_EQ(concat4Val, "Withers, L.");
}

TEST(BLAZESUITE, Pos) {
    NonTerminal dummyNonTerm("dummy", Helper::String);
    NonTerminal tokenNonTerm("ntToken", Helper::String);
    NonTerminal ntInt("ntInt", Helper::Int);
    NonTerminal ntDir("ntDir", Helper::Int);
    //Pos(x, 'ProperCase', -1, 1))
    std::string input = "Launa|Withers";
    auto x = new StringLiteral(1, input, dummyNonTerm);
    auto oneDir = new IntLiteral(1, 1, ntDir);
    auto propercase = new StringLiteral(1, "ProperCase", tokenNonTerm);
    auto minusOne = new IntLiteral(1, -1, ntInt);
    auto pos = new Pos(x, propercase, minusOne, oneDir, dummyNonTerm);


    //Pos(x, 'ProperCase', 2, 1))
    auto two = new IntLiteral(1, 2, ntInt);
    auto pos2 = new Pos(x, propercase, two, oneDir, dummyNonTerm);

    EXPECT_EQ(pos->getValues().at(0), pos2->getValues().at(0));
}

TEST(BLAZESUITE, Pos2) {
    std::string input = "Launa|Withers";
    std::string expectedOutput = "Withers, L.";
    // SubStr(x, Pos(x, 'Sep', 1, 1), Pos(x, 'ProperCase', -1, 1)
    NonTerminal dummyNonTerm("dummy", Helper::String);
    NonTerminal tokenNonTerm("ntToken", Helper::String);
    NonTerminal ntInt("ntInt", Helper::Int);
    NonTerminal ntDir("ntDir", Helper::Int);

    auto x = new StringLiteral(1, input, dummyNonTerm);
    auto sep = new StringLiteral(1, "Alphabets", tokenNonTerm);
    auto k = new IntLiteral(1, 2, ntInt);
    auto d = new IntLiteral(1, 1, ntDir);
    auto pos1 = new Pos(x, sep, k, d, dummyNonTerm);
    auto pos1Val = std::get<int>(pos1->getValues().at(0));
    EXPECT_EQ(pos1Val, 13);

}


TEST(BLAZESUITE, Substr) {
    std::string input = "UCLA|Los Angeles, CA";
    std::string expectedOutput = ERRSTRING;
    //(Str (SubStr x (Pos x, "ProperCase", 3, 0) (Pos x, "CAPS", -1, 1)))
    NonTerminal dummyNonTerm("dummy", Helper::String);
    NonTerminal tokenNonTerm("ntToken", Helper::String);
    NonTerminal ntInt("ntInt", Helper::Int);
    NonTerminal ntDir("ntDir", Helper::Int);

    auto x = new StringLiteral(1, input, dummyNonTerm);
    auto properCase = new StringLiteral(1, "ProperCase", tokenNonTerm);
    auto caps = new StringLiteral(1, "CAPS", tokenNonTerm);
    auto k3 = new IntLiteral(1, 3, ntInt);
    auto kMinus1 = new IntLiteral(1, -1, ntInt);
    auto d0 = new IntLiteral(1, 0, ntDir);
    auto d1 = new IntLiteral(1, 1, ntDir);

    auto pos1 = new Pos(x, properCase, k3, d0, dummyNonTerm);
    auto pos2 = new Pos(x, caps, kMinus1, d1, dummyNonTerm);

    auto substr = new Substr(x, pos1, pos2, dummyNonTerm);
    EXPECT_EQ(std::get<std::string>(substr->getValues().at(0)), expectedOutput);
}