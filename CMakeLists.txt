cmake_minimum_required(VERSION 3.27)
include(FetchContent)
project(merlin)

set(CMAKE_CXX_STANDARD 23)

#### ANTLR
# set antlr runtime location
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.13.1-complete.jar)

# antlr runtime
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/antlr_cmake) # add ./cmake folder to cmake module path
find_package(ANTLR REQUIRED) # require antlr commands
antlr_target(sygusparser ${CMAKE_CURRENT_SOURCE_DIR}/Sygus.g4 LEXER PARSER VISITOR) # generate parser for logic -> output: ${ANTLR_sygusparser_CXX_OUTPUTS}
add_definitions(-DANTLR4CPP_STATIC) # required if linking to static library
set(ANTLR4_WITH_STATIC_CRT OFF) # using /MD flag for antlr4_runtime (for Visual C++ compilers only)
include(ExternalAntlr4Cpp) # build and add runtime for antlr4 -> variable: antlr4_static

FIND_PACKAGE( Boost 1.83 COMPONENTS log program_options REQUIRED)

#FIND_PACKAGE(cvc5 REQUIRED)

find_library(LIBMATA mata REQUIRED)

FetchContent_Declare(z3
        GIT_REPOSITORY https://github.com/Z3Prover/z3
        GIT_TAG        z3-4.14.1
)
FetchContent_MakeAvailable(z3)
include_directories(${z3_SOURCE_DIR}/src/api ${z3_SOURCE_DIR}/src/api/c++)

include_directories( ${PROJECT_BINARY_DIR})

# add generated source files
add_library( ${PROJECT_NAME}_code STATIC
        ${ANTLR_sygusparser_CXX_OUTPUTS}
        Helper.cpp
        Helper.h
        thirdparty/magic_enum/magic_enum.hpp
        IOExample.cpp
        IOExample.h
        ast/ASTNode.cpp
        ast/ASTNode.h
        ast/BinaryOpNode.cpp
        ast/BinaryOpNode.h
        ast/BinaryOpNodes/IntAddition.cpp
        ast/BinaryOpNodes/IntAddition.h
        ast/LiteralNode.cpp
        ast/LiteralNode.h
        ast/LiteralNodes/StringLiteral.cpp
        ast/LiteralNodes/StringLiteral.h
        ast/LiteralNodes/IntLiteral.cpp
        ast/LiteralNodes/IntLiteral.h
        ast/LiteralNodes/BoolLiteral.cpp
        ast/LiteralNodes/BoolLiteral.h
        ast/LiteralNodes/BVLiteral.cpp
        ast/LiteralNodes/BVLiteral.h
        ast/BinaryOpNodes/StringConcat.cpp
        ast/BinaryOpNodes/StringConcat.h
        ast/BinaryOpNodes/StringAt.cpp
        ast/BinaryOpNodes/StringAt.h
        ast/BinaryOpNodes/IntSubstraction.cpp
        ast/BinaryOpNodes/IntSubstraction.h
        ast/BinaryOpNodes/IntLessThanEq.cpp
        ast/BinaryOpNodes/IntLessThanEq.h
        ast/BinaryOpNodes/BoolEquals.cpp
        ast/BinaryOpNodes/BoolEquals.h
        ast/BinaryOpNodes/IntEquals.cpp
        ast/BinaryOpNodes/IntEquals.h
        ast/BinaryOpNodes/PrefixOf.cpp
        ast/BinaryOpNodes/PrefixOf.h
        ast/BinaryOpNodes/SuffixOf.cpp
        ast/BinaryOpNodes/SuffixOf.h
        ast/BinaryOpNodes/Contains.cpp
        ast/BinaryOpNodes/Contains.h
        ast/BinaryOpNodes/BVAnd.cpp
        ast/BinaryOpNodes/BVAnd.h
        ast/BinaryOpNodes/BVOr.cpp ast/BinaryOpNodes/BVOr.h
        ast/BinaryOpNodes/BVXor.cpp
        ast/BinaryOpNodes/BVXor.h
        ast/BinaryOpNodes/BVShiftLeft.cpp
        ast/BinaryOpNodes/BVShiftLeft.h
        ast/BinaryOpNodes/BVAdd.cpp
        ast/BinaryOpNodes/BVAdd.h
        ast/BinaryOpNodes/BVSub.cpp
        ast/BinaryOpNodes/BVSub.h
        ast/BinaryOpNodes/BVSDiv.cpp
        ast/BinaryOpNodes/BVSDiv.h
        ast/BinaryOpNodes/BVUDiv.cpp
        ast/BinaryOpNodes/BVUDiv.h
        ast/BinaryOpNodes/BVMul.cpp
        ast/BinaryOpNodes/BVMul.h
        ast/BinaryOpNodes/BVShrLogical.cpp
        ast/BinaryOpNodes/BVShrLogical.h
        ast/BinaryOpNodes/BVShrArithmetic.cpp
        ast/BinaryOpNodes/BVShrArithmetic.h
        ast/BinaryOpNodes/BVEquals.cpp
        ast/BinaryOpNodes/BVEquals.h
        ast/BinaryOpNodes/BVSlt.cpp
        ast/BinaryOpNodes/BVSlt.h
        ast/BinaryOpNodes/BVSle.cpp
        ast/BinaryOpNodes/BVSle.h
        ast/BinaryOpNodes/BVUlt.cpp
        ast/BinaryOpNodes/BVUlt.h
        ast/BinaryOpNodes/BVUle.cpp
        ast/BinaryOpNodes/BVUle.h
        ast/BinaryOpNodes/BVUgt.cpp
        ast/BinaryOpNodes/BVUgt.h
        ast/BinaryOpNodes/LAnd.cpp
        ast/BinaryOpNodes/LAnd.h
        ast/BinaryOpNodes/LOr.cpp
        ast/BinaryOpNodes/LOr.h
        ast/BinaryOpNodes/LXor.cpp
        ast/BinaryOpNodes/LXor.h
        ast/BinaryOpNodes/BVSRem.cpp
        ast/BinaryOpNodes/BVSRem.h
        ast/BinaryOpNodes/BVURem.cpp
        ast/BinaryOpNodes/BVURem.h
        ast/TernaryOpNode.cpp
        ast/TernaryOpNode.h
        ast/TernaryOpNodes/StringReplace.cpp
        ast/TernaryOpNodes/StringReplace.h
        ast/TernaryOpNodes/StringITE.cpp
        ast/TernaryOpNodes/StringITE.h
        ast/TernaryOpNodes/IntITE.cpp
        ast/TernaryOpNodes/IntITE.h
        ast/TernaryOpNodes/Substring.cpp
        ast/TernaryOpNodes/Substring.h
        ast/TernaryOpNodes/IndexOf.cpp
        ast/TernaryOpNodes/IndexOf.h
        ast/TernaryOpNodes/BVITE.cpp
        ast/TernaryOpNodes/BVITE.h
        ast/UnaryOpNode.cpp
        ast/UnaryOpNode.h
        ast/UnaryOpNodes/IntToString.cpp
        ast/UnaryOpNodes/IntToString.h
        ast/UnaryOpNodes/StringToInt.cpp
        ast/UnaryOpNodes/StringToInt.h
        ast/UnaryOpNodes/StringLength.cpp
        ast/UnaryOpNodes/StringLength.h
        ast/UnaryOpNodes/BVNot.cpp
        ast/UnaryOpNodes/BVNot.h
        ast/UnaryOpNodes/BVNeg.cpp
        ast/UnaryOpNodes/BVNeg.h
        ast/UnaryOpNodes/LNot.cpp
        ast/UnaryOpNodes/LNot.h
        ast/UnaryOpNodes/BVRedor.cpp
        ast/UnaryOpNodes/BVRedor.h
        ast/VariableNode.cpp
        ast/VariableNode.h
        ast/VariableNodes/StringVariable.cpp
        ast/VariableNodes/StringVariable.h
        ast/VariableNodes/IntVariable.cpp
        ast/VariableNodes/IntVariable.h
        ast/VariableNodes/BoolVariable.cpp
        ast/VariableNodes/BoolVariable.h
        ast/VariableNodes/BVVariable.cpp
        ast/VariableNodes/BVVariable.h
        VocabMaker.cpp
        VocabMaker.h
        enumeration/OEValuesManager.cpp
        enumeration/OEValuesManager.h
        VocabFactory.cpp
        VocabFactory.h
        SyGuSFileTask.cpp
        SyGuSFileTask.h
        ast/BinaryOpNodes/IntGreaterThanEq.cpp
        ast/BinaryOpNodes/IntGreaterThanEq.h
        Solver.cpp
        Solver.h
        SMTProcesses/SMTProcess.cpp
        SMTProcesses/SMTProcess.h
        enumeration/SizeBasedEnumerator.cpp
        enumeration/SizeBasedEnumerator.h
        enumeration/SizeBasedChildrenIterator.cpp
        enumeration/SizeBasedChildrenIterator.h
        enumeration/Enumerator.cpp
        enumeration/Enumerator.h
        enumeration/SizeVectorIterator.cpp
        enumeration/SizeVectorIterator.h
        SMTProcesses/Z3SMTProcess.cpp
        SMTProcesses/Z3SMTProcess.h
        #SMTProcesses/CVC5SMTProcess.cpp
        #SMTProcesses/CVC5SMTProcess.h
        NonTerminal.cpp
        NonTerminal.h
        ast/Blaze/Str.cpp
        ast/Blaze/Str.h
        ast/Blaze/Pos.cpp
        ast/Blaze/Pos.h
        ast/QuadraryOpNode.cpp
        ast/QuadraryOpNode.h
        ast/Blaze/Substr.cpp
        ast/Blaze/Substr.h
        ast/Blaze/ConstStr.cpp
        ast/Blaze/ConstStr.h
        ast/Blaze/Concat.cpp
        ast/Blaze/Concat.h
        ast/Blaze/ConstPos.cpp
        ast/Blaze/ConstPos.h
        Config.cpp
        Config.h
        enumeration/SizeBasedEnumeratorXorAdd.cpp
        enumeration/SizeBasedEnumeratorXorAdd.h
        enumeration/SizeBasedEnumeratorBottomUpDeductionUnderApprox.cpp
        enumeration/SizeBasedEnumeratorBottomUpDeductionUnderApprox.h
        enumeration/SizeBasedEnumeratorAndUnderApprox.cpp
        enumeration/SizeBasedEnumeratorAndUnderApprox.h
        enumeration/SizeBasedEnumeratorOrUnderApprox.cpp
        enumeration/SizeBasedEnumeratorOrUnderApprox.h
        enumeration/SizeBasedEnumeratorMulUnderApprox.cpp
        enumeration/SizeBasedEnumeratorMulUnderApprox.h
        enumeration/SizeBasedEnumeratorHDUnderApprox.cpp
        enumeration/SizeBasedEnumeratorHDUnderApprox.h
        enumeration/SizeBasedPortfolio.cpp
        enumeration/SizeBasedPortfolio.h
        CustomVocabMaker.cpp
        CustomVocabMaker.h
        enumeration/SizeBasedEnumerationXorAddHDUnderApprox.cpp
        enumeration/SizeBasedEnumerationXorAddHDUnderApprox.h
        BottomUpDeduction/BottomUpDeduction.h
        BottomUpDeduction/AndBottomUpDeduction.cpp
        BottomUpDeduction/AndBottomUpDeduction.h
        BottomUpDeduction/OrBottomUpDeduction.cpp
        BottomUpDeduction/OrBottomUpDeduction.h
        BottomUpDeduction/MulBottomUpDeduction.cpp
        BottomUpDeduction/MulBottomUpDeduction.h
        BottomUpDeduction/XorAddBottomUpDeduction.cpp
        BottomUpDeduction/XorAddBottomUpDeduction.h
        enumeration/SizeBasedCombinedBottomUp.cpp
        enumeration/SizeBasedCombinedBottomUp.h
        enumeration/SizeBasedEnumerationConcatUnderApprox.cpp
        enumeration/SizeBasedEnumerationConcatUnderApprox.h
        enumeration/SizeBasedEnumerationSubstrUnderApprox.cpp
        enumeration/SizeBasedEnumerationSubstrUnderApprox.h
        enumeration/SizePortfolioString.cpp
        enumeration/SizePortfolioString.h
        enumeration/SizePortfolioBV.cpp
        enumeration/SizePortfolioBV.h
        enumeration/SizeBasedEnumerationReplaceUnderApprox.cpp
        enumeration/SizeBasedEnumerationReplaceUnderApprox.h
        LevenshteinAutomatonCreator.cpp
        LevenshteinAutomatonCreator.h
        BottomUpDeduction/ConcatBottomUpDeduction.cpp
        BottomUpDeduction/ConcatBottomUpDeduction.h
        BottomUpDeduction/BottomUpDeductionBV.h
        BottomUpDeduction/BottomUpDeductionString.h
        BottomUpDeduction/SubstrBottomUpDeduction.cpp
        BottomUpDeduction/SubstrBottomUpDeduction.h
        BottomUpDeduction/ReplBottomUpDeduction.cpp
        BottomUpDeduction/ReplBottomUpDeduction.h
        enumeration/SizeBasedCombinedBottomUpStrings.cpp
        enumeration/SizeBasedCombinedBottomUpStrings.h
        example.cpp
)

target_include_directories(${PROJECT_NAME}_code PRIVATE ${Boost_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME}_code PUBLIC ${ANTLR4_INCLUDE_DIRS} ${ANTLR_sygusparser_OUTPUT_DIR})
target_link_libraries(${PROJECT_NAME}_code PUBLIC antlr4_static) # link with antlr runtime
include_directories("/usr/local/include")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries( ${PROJECT_NAME}_code PUBLIC
        libz3
        PUBLIC
        ${Boost_LOG_LIBRARY}
        PUBLIC
        ${Boost_LIBRARIES}
        PUBLIC
        ${LIBMATA}
)

target_link_libraries( ${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_code
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

######### TESTS ###############
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)


add_subdirectory(Tests)
