
#ifndef MYSOLVER_HELPER_H
#define MYSOLVER_HELPER_H
#include "Config.h"
#include "string"
#include "variant"
#include "SygusParser.h"

typedef std::variant<std::string, int, bool, uint64_t> variant;

class Helper {
public: enum Type { String, Int, Bool, BitVec64 };
        enum Logic { LIA, SLIA, BV, SLIA_PBE };

        enum EnumerationMode {
                Size, SizeBottomUpXorAdd,
                SizeBottomUpAndUnderApprox, SizeBottomUpOrUnderApprox, SizeBottomUpMulUnderApprox,
                SizeBottomUpHDUnderApprox, SizePortfolioBV, SizePortfolioString,
                SizeBottomUpXorAddHDUnderApprox,
                SizeBasedCombinedBottomUp,
                SizeBasedCombinedBottomUpStrings,
                SizeBottomUpConcatUnderApprox, SizeBottomUpSubstrUnderApprox,
                SizeBottomUpReplaceUnderApprox
        };
        enum SMTSolver {Z3, CVC5};

        /**
         *
         * @param literal parser literal
         * @param returnType type of var to return
         * @return the literal converted to the specified type
         */
        static variant literalToAny(SygusParser::LiteralContext* literal, Helper::Type returnType);
        /**
         *
         * @param input variant to convert to string
         * @return string representation of input
         */
        static std::string variantToString(variant input);

        /**
         *
         * @param input vector of variants
         * @return string representation of input vector
         */
        static std::string variantToString(const std::vector<variant>& input);

        /**
         *
         * @param type type to generate string of
         * @return string of type name
         */
        static std::string typeToSmtString(Helper::Type type);
        /**
         *
         * @param val bitvector
         * @return string representation of bitvector
         */
        static std::string bvToSmtString(uint64_t val);
        /**
         *
         * @param val variant to get type of
         * @return returns type of given variant
         */
        static Helper::Type typeOf(const variant& val);
        /**
         *
         * @param context context to convert to string
         * @return string representation of context
         */
        static std::string contextToString(const std::map<std::string, variant> &context);

        /**
         *
         * @param a bitvector to count ones of
         * @return counts ones in bitvector
         */
        static int countOnes(uint64_t a);

        /**
         *
         * @param a bitvec a
         * @param b bitvec b
         * @return hamming distance between a and b
         */
        static int hammingDistance(uint64_t a, uint64_t b);

        /**
         *
         * @param a bitvec a
         * @return counts number of trailing zeros in a
         */
        static int countTrailingZeros(uint64_t a);

        /**
         *
         * @param argc arg counts from terminal
         * @param argv arguments from terminal
         * @param config config object to write options into
         */
        static void parseProgramOptions(int argc, const char **argv, Config *config);
};


#endif //MYSOLVER_HELPER_H
