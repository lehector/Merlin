#include "Config.h"
#include "Solver.h"

#include <boost/log/trivial.hpp>

void runExample() {
    Config* config = Config::GetInstance();
    std::string filename = "../benchmarks/example/example_all.sl";
    config->setReplComplete(false);
    config->setDomain("string");
    config->setFileName(filename);
    config->setCutoffSize(1);
    config->setFirstNValuesCheck(3);
    config->setMaxSize(7);

    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    BOOST_LOG_TRIVIAL(info) << "No Pruning / Factorization";
    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    config->setMaxContexts(100);
    config->setOePruning(false);
    config->setViolation(false);
    config->setReuseComponents(false);

    Solver solver1(filename, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox, Helper::SMTSolver::Z3, true);
    solver1.solve();

    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    BOOST_LOG_TRIVIAL(info) << "OE Factorization";
    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    config->setMaxContexts(100);
    config->setOePruning(true);
    config->setViolation(false);
    config->setReuseComponents(false);

    Solver solver2(filename, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox, Helper::SMTSolver::Z3, true);
    solver2.solve();

    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    BOOST_LOG_TRIVIAL(info) << "Metric Pruning";
    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    config->setMaxContexts(100);
    config->setOePruning(false);
    config->setViolation(true);
    config->setReuseComponents(false);

    Solver solver3(filename, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox, Helper::SMTSolver::Z3, true);
    solver3.solve();

    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    BOOST_LOG_TRIVIAL(info) << "OE Factorization + Metric Pruning";
    BOOST_LOG_TRIVIAL(info) << "==========================================================";
    config->setMaxContexts(100);
    config->setOePruning(true);
    config->setViolation(true);
    config->setReuseComponents(false);

    Solver solver4(filename, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox, Helper::SMTSolver::Z3, true);
    solver4.solve();

    BOOST_LOG_TRIVIAL(info) << "========================================================================";
    BOOST_LOG_TRIVIAL(info) << "Abstraction + OE Factorization + Metric Pruning + Learning -- First Run";
    BOOST_LOG_TRIVIAL(info) << "========================================================================";
    config->setMaxContexts(1);
    config->setOePruning(true);
    config->setViolation(true);
    config->setReuseComponents(true);
    config->setMaxSize(4);

    Solver solver5(filename, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox, Helper::SMTSolver::Z3, true);
    solver5.solve();


    BOOST_LOG_TRIVIAL(info) << "========================================================================";
    BOOST_LOG_TRIVIAL(info) << "Abstraction + OE Factorization + Metric Pruning + Learning -- Second Run";
    BOOST_LOG_TRIVIAL(info) << "========================================================================";
    config->setMaxContexts(1);
    config->setOePruning(true);
    config->setViolation(true);
    config->setReuseComponents(true);
    config->setMaxSize(4);
    filename = "../benchmarks/example/example_learning.sl";
    config->setFileName(filename);

    Solver solver6(filename, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox, Helper::SMTSolver::Z3, true);
    solver6.solve();
}
