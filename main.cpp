#include <iostream>
#include "Solver.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "Config.h"
#include "example.cpp"

bool fileExists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, const char* argv[]) {
    Config* config = Config::GetInstance();

    Helper::parseProgramOptions(argc, argv, config);
    if (config->isRunExample()) {
        runExample();
        exit(0);
    }

    std::string filename;
    //filename = "../benchmarks/bitvec/5_1000.sl";
    //filename = "../benchmarks/blaze/univ_1-long.sl";
    //filename = "../benchmarks/duet_strings/17212077.sl";
    //filename = "../benchmarks/string-2018/univ_3-long.sl";
    //filename = "../benchmarks/duet_string_no_ite_string/univ_3-long.sl";
    //filename = "../benchmarks/not-from-probe/paperEx.sl";
    //filename = "../benchmarks/hackers-delight/hd-20.sl";
    //filename = "../benchmarks/probe_extra_hd/hd-22.sl";
    //filename = "../benchmarks/not-from-probe/9xplus3yminus3z.sl"; // very good for cegar test
    //filename = "../benchmarks/not-from-probe/small_ex.sl"; // very good for cegar test
    //filename = "../benchmarks/deobfusc/target_32.sl";
    //filename = "../benchmarks/not-from-probe/bitvector.sl";
    //filename = "../benchmarks/simba_hd/hd-04-d0-prog.sl";

    //bool useNewFeatures = true;

    //config->setMaxContexts(1);
    //config->setOePruning(true);

    //config->setReplComplete(false);

    //config->setDomain("string");
    //config->setFileName(filename);
    //config->setCutoffSize(3);
    //config->setFirstNValuesCheck(3);
    //config->setViolation(true);
    //config->setReuseComponents(true);
    //config->setMaxContexts(1);

    //config->setForceImprovement(false);


    //config->setFileName(filename);
    //config->setCutoffSize(7);
    //config->setFirstNValuesCheck(3);
    //config->setViolation(true);
    //config->setReuseComponents(true);
    //config->setDomain("bv");
    //config->setMaxContexts(2);
    //config->setOePruning(true);

    //config->setForceImprovement(false);


    //config->setAndOrRadius(100);
    //config->setMulRadius(100);
    //config->setHdRadius(100);

    //config->setConcatSubstrRadius(100);
    //config->setLevenshteinRadius(4);

    if(!fileExists(config->getFileName())) {
        BOOST_LOG_TRIVIAL(error) << "File Not Found!: " << config->getFileName();
        exit(1);
    }

    bool disableIte = true;

    Helper::EnumerationMode enumerationMode;
    if (config->getDomain() == "bv") {
        if (config->isViolation()) {
            enumerationMode = Helper::EnumerationMode::SizePortfolioBV;
        }
        else {
            enumerationMode = Helper::EnumerationMode::SizeBasedCombinedBottomUp;
        }
    }
    else if (config->getDomain() == "string") {
        if (config->isViolation()) {
            enumerationMode = Helper::EnumerationMode::SizePortfolioString;
        }
        else {
            enumerationMode = Helper::EnumerationMode::SizeBasedCombinedBottomUpStrings;
        }
    }
    else {
        assert(false && "enumeration mode must be set");
        BOOST_LOG_TRIVIAL(error) << "Domain not valid" << config->getDomain();
        exit(1);
    }

    Solver solver(config->getFileName(), enumerationMode, Helper::SMTSolver::Z3, disableIte);


    solver.printInfo();
    solver.solve();
    //solver.printSolution();
    return 0;
}
