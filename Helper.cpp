
#include "Helper.h"
#include "thirdparty/magic_enum/magic_enum.hpp"
#include "iomanip"
#include <boost/program_options.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


void initBoostLog(bool silent)
{
    if (silent) {
        boost::log::core::get()->set_filter
                (
                        boost::log::trivial::severity > boost::log::trivial::info
                );
    }
    else {
        boost::log::core::get()->set_filter
               (
                       boost::log::trivial::severity >= boost::log::trivial::info
               );
    }
}

bool to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

variant Helper::literalToAny(SygusParser::LiteralContext *literal, Helper::Type returnType) {
    switch (returnType) {
        case Helper::Type::Int:
            return std::stoi(literal->Numeral()->getText());
        case Helper::Type::String: {
            std::string mystring = literal->StringConst()->getSymbol()->getText().substr(1); // delete first char
            mystring.pop_back(); // delete last char
            return mystring;
        }
        case Helper::Type::Bool:
            return to_bool(literal->BoolConst()->toString());
        case Helper::Type::BitVec64: {
            if(literal->HexConst() != nullptr) {
                // hexadecimal
                return std::stoull(literal->HexConst()->getText().substr(2), nullptr, 16); // TODO maybe get uint64
            }
            else {
                // binary
                return std::stoull(literal->BinConst()->getText().substr(2), nullptr, 2);
            }
        }
    }
}

std::string Helper::variantToString(variant val) {
    std::stringstream ss;
    if (const auto intPtr (std::get_if<int>(&val)); intPtr)
        ss << *intPtr;
    else if (const auto boolPtr (std::get_if<bool>(&val)); boolPtr)
        ss << std::boolalpha << *boolPtr;
    else if (const auto ullPtr (std::get_if<uint64_t>(&val)); ullPtr)
        ss << Helper::bvToSmtString(*ullPtr);
    else if (const auto stringPtr (std::get_if<std::string>(&val)); stringPtr)
        ss << *stringPtr;
    assert((!ss.str().empty() || std::get<std::string>(val) == "" ) && "has to be one of the types");
    return ss.str();
}

std::string Helper::variantToString(const std::vector<variant>& input) {
    std::stringstream ss;
    ss << "(";
    for(int i = 0; i < input.size(); i++) {
        ss << Helper::variantToString(input.at(i));
        if(i != input.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
}

std::string Helper::typeToSmtString(Helper::Type type) {
    std::string str;
    if(type == Helper::Type::BitVec64) {
        str = "(_ BitVec 64)";
    }
    else {
        str = magic_enum::enum_name(type);
    }
    return str;
}

std::string Helper::bvToSmtString(uint64_t val) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(16) << std::hex << val;
    return "#x" + ss.str();
}

Helper::Type Helper::typeOf(const variant& val) {
    if (std::holds_alternative<int>(val)) {
        return Helper::Type::Int;
    }
    if (std::holds_alternative<uint64_t>(val)) {
        return Helper::Type::BitVec64;
    }
    if (std::holds_alternative<bool>(val)) {
        return Helper::Type::Bool;
    }
    if (std::holds_alternative<std::string>(val)) {
        return Helper::Type::String;
    }
    assert(false && "there must be a matching type");
}

std::string Helper::contextToString(const std::map<std::string, variant> &context) {
    std::stringstream ss;
    for(const auto& [varName, val] : context) {
        ss << varName << ": " << variantToString(val) << "; ";
    }
    return ss.str();
}

int Helper::countOnes(uint64_t a){
    int count = 0;
    while (a != 0) {
        count += 1;
        a &= ( a - 1); // equals to `xorVal = xorVal & ( xorVal - 1);`
    }
    return count;
}

int Helper::hammingDistance(uint64_t a, uint64_t b){
    uint64_t xorVal = a ^ b;
    return countOnes(xorVal);
}

int Helper::countTrailingZeros(uint64_t a) {
    int count = 0;
    for(int i = 0; i < 64; i++) {
        uint64_t lastBit = a & 1;
        if(lastBit == 0) count++;
        else break;
        a = a >> 1;
    }
    return count;
}

void Helper::parseProgramOptions(int argc, const char **argv, Config *config) {
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("run-example,re", boost::program_options::value<bool>(), "whether to run the example")
            ("silent,s", boost::program_options::value<bool>(), "whether to use silent mode, default = 0")
            ("input-file,i", boost::program_options::value<std::string>(), "input file")
            ("check-violation,v", boost::program_options::value<bool>(), "check violation if enabled")
            ("use-oe,o", boost::program_options::value<bool>(), "use observational equivalence, default = 1")
            ("cutoff-size,c", boost::program_options::value<int>(), "cut off size, for check violation")
            ("max-contexts,m", boost::program_options::value<int>(), "how many contexts should be considered in first iteration")
            ("n-values-check,n", boost::program_options::value<int>(), "how many values to check before creating object instance, default = 3")
            ("reuse-components,r", boost::program_options::value<bool>(), "reuse components if enabled")
            ("domain,d", boost::program_options::value<std::string>(), "domain: either \"string\" or \"bv\"")
            ("replace-complete, rc", boost::program_options::value<bool>(), "be complete with replace, default = 0")
            ("and-or-radius, aor", boost::program_options::value<int>(), "radius for and / or, default = 100")
            ("mul-radius, mr", boost::program_options::value<int>(), "radius for mul, default  = 100")
            ("hd-radius, hdr", boost::program_options::value<int>(), "radius for hd, default = 66")
            ("concat-substr-radius, csr", boost::program_options::value<int>(), "radius for concat / substr, default = 100")
            ("levenshtein-radius, lr", boost::program_options::value<int>(), "radius for levenshtein, default = 4")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(1);
    }

    bool silent = false;
    if (vm.count("silent")) {
        silent = vm["silent"].as<bool>();
        initBoostLog(silent);
        if (!silent) {
            BOOST_LOG_TRIVIAL(info) << "Silent mode set to "
                    << std::boolalpha << vm["silent"].as<bool>() << ".";
        }
    } else {
        BOOST_LOG_TRIVIAL(info) << "Silent mode was not set";
        BOOST_LOG_TRIVIAL(info) << "Using default (0)";
    }
    config->setSilent(silent);

    if (vm.count("run-example")) {
        bool run_example = vm["run-example"].as<bool>();
        if (!run_example) {
            BOOST_LOG_TRIVIAL(info) << "Run Example mode set to "
                    << std::boolalpha << vm["run-example"].as<bool>() << ".";
        }
        config->setRunExample(run_example);
    }
    else {
        config->setRunExample(false);
    }

    if (vm.count("input-file")) {
        BOOST_LOG_TRIVIAL(info) << "Input File set to "
                << vm["input-file"].as<std::string>() << ".";
        config->setFileName(vm["input-file"].as<std::string>());
    } else {
        std::cout << "Input File was not set.\n";
        std::cout << desc << "\n";
        exit(-1);
    }
    if (vm.count("domain")) {
        BOOST_LOG_TRIVIAL(info) << "Domain set to "
                << vm["domain"].as<std::string>() << ".";
        std::string domain = vm["domain"].as<std::string>();
        if (domain == "string") {
            config->setDomain(domain);
        }
        else if (domain == "bv") {
            config->setDomain("bv");
        }
        else {
            std::cout << "Domain must be either \"string\" or \"bv\".\n";
            std::cout << desc << "\n";
            exit(-1);
        }
    } else {
        std::cout << "Domain was not set.\n";
        std::cout << desc << "\n";
        exit(-1);
    }
    if (vm.count("check-violation")) {
        BOOST_LOG_TRIVIAL(info) << "check-violation set to "
                << vm["check-violation"].as<bool>() << ".";
        config->setViolation(vm["check-violation"].as<bool>());
        if (vm.count("cutoff-size")) {
            BOOST_LOG_TRIVIAL(info) << "cutoff-size set to "
                    << vm["cutoff-size"].as<int>() << ".";
            config->setCutoffSize(vm["cutoff-size"].as<int>());
        } else {
            std::cout << "cutoff size was not set.\n";
            std::cout << desc << "\n";
            exit(-1);
        }
    }
    else {
        std::cout << "check-violation was not set.\n";
        std::cout << desc << "\n";
        exit(-1);
    }
    if (vm.count("n-values-check")) {
        BOOST_LOG_TRIVIAL(info) << "n-values-check set to "
                << vm["n-values-check"].as<int>() << ".";
        config->setFirstNValuesCheck(vm["n-values-check"].as<int>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "n values check was not set.";
        BOOST_LOG_TRIVIAL(info) << "setting n-values-check to default (3) ";
        config->setFirstNValuesCheck(3);
    }
    if (vm.count("reuse-components")) {
        BOOST_LOG_TRIVIAL(info) << "reuse-components set to "
                << vm["reuse-components"].as<bool>() << ".";
        config->setReuseComponents(vm["reuse-components"].as<bool>());
    } else {
        std::cout << "reuse components was not set.\n";
        std::cout << desc << "\n";
        exit(-1);
    }
    if (vm.count("use-oe")) {
        BOOST_LOG_TRIVIAL(info) << "use-oe set to "
                << vm["use-oe"].as<bool>() << ".";
        config->setOePruning(vm["use-oe"].as<bool>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "use-oe was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (1) ";
        config->setOePruning(true);
    }
    if (vm.count("replace-complete")) {
        BOOST_LOG_TRIVIAL(info) << "replace-complete set to "
                << vm["replace-complete"].as<bool>() << ".";
        config->setReplComplete(vm["replace-complete"].as<bool>());
    } else {
        // default
        BOOST_LOG_TRIVIAL(info) << "replace-complete was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (0) ";
        config->setReplComplete(false);
    }
    if (vm.count("max-contexts")) {
        BOOST_LOG_TRIVIAL(info) << "max-contexts set to "
                << vm["max-contexts"].as<int>() << ".";
        config->setMaxContexts(vm["max-contexts"].as<int>());
    } else {
        std::cout << "max-contexts was not set.\n";
        std::cout << desc << "\n";
        exit(-1);
    }
    if (vm.count("and-or-radius")) {
        BOOST_LOG_TRIVIAL(info) << "and-or-radius set to "
                << vm["and-or-radius"].as<int>() << ".";
        config->setAndOrRadius(vm["and-or-radius"].as<int>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "and-or-radius was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (100)";
        config->setAndOrRadius(100);
    }
    if (vm.count("hd-radius")) {
        BOOST_LOG_TRIVIAL(info) << "hd-radius set to "
                << vm["hd-radius"].as<int>() << ".";
        config->setHdRadius(vm["hd-radius"].as<int>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "hd-radius was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (66)";
        config->setHdRadius(66);
    }
    if (vm.count("mul-radius")) {
        BOOST_LOG_TRIVIAL(info) << "mul-radius set to "
                << vm["mul-radius"].as<int>() << ".";
        config->setMulRadius(vm["mul-radius"].as<int>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "mul-radius was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (100)";
        config->setMulRadius(100);
    }
    if (vm.count("concat-substr-radius")) {
        BOOST_LOG_TRIVIAL(info) << "concat-substr-radius set to "
                << vm["concat-substr-radius"].as<int>() << ".";
        config->setConcatSubstrRadius(vm["concat-substr-radius"].as<int>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "concat-substr-radius was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (100)";
        config->setConcatSubstrRadius(100);
    }
    if (vm.count("levenshtein-radius")) {
        BOOST_LOG_TRIVIAL(info) << "levenshtein-radius set to "
                << vm["levenshtein-radius"].as<int>() << ".";
        config->setLevenshteinRadius(vm["levenshtein-radius"].as<int>());
    } else {
        BOOST_LOG_TRIVIAL(info) << "levenshtein-radius was not set.";
        BOOST_LOG_TRIVIAL(info) << "set to default (4)";
        config->setLevenshteinRadius(4);
    }
}
