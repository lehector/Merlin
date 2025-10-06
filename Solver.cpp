
#include "Solver.h"

#include <future>

#include "Config.h"

#include <regex>

#include "thirdparty/magic_enum/magic_enum.hpp"
#include "SMTProcesses/Z3SMTProcess.h"
//#include "SMTProcesses/CVC5SMTProcess.h"
#include <boost/log/trivial.hpp>

#include "enumeration/SizeBasedCombinedBottomUp.h"
#include "enumeration/SizeBasedCombinedBottomUpStrings.h"
#include "enumeration/SizeBasedEnumerationConcatUnderApprox.h"
#include "enumeration/SizeBasedEnumerationReplaceUnderApprox.h"
#include "enumeration/SizeBasedEnumerationSubstrUnderApprox.h"
#include "enumeration/SizeBasedEnumerationXorAddHDUnderApprox.h"
#include "enumeration/SizeBasedEnumeratorAndUnderApprox.h"
#include "enumeration/SizeBasedEnumeratorHDUnderApprox.h"
#include "enumeration/SizeBasedEnumeratorMulUnderApprox.h"
#include "enumeration/SizeBasedEnumeratorOrUnderApprox.h"
#include "enumeration/SizeBasedEnumeratorXorAdd.h"
#include "enumeration/SizeBasedPortfolio.h"
#include "enumeration/SizePortfolioBV.h"
#include "enumeration/SizePortfolioString.h"


void Solver::generateExamplesFromFuncSpec(ASTNode *func, const std::vector<std::pair<std::string, Helper::Type>> &vars, int n, std::mt19937 gen, std::uniform_int_distribution<unsigned long long> dis) {
    std::vector<uint64_t> masks = {UINT64_MAX, 0xFFFF, 0x3f}; // more diverse inputs
    for (int i = 0; i < n; i++) {
        std::map<std::string, variant> context;
        uint index = dis(gen) % masks.size();
        for (const std::pair<std::string, Helper::Type> &var: vars) {
            uint64_t mask = masks[index];
            index = (index + 1) % masks.size();
            uint64_t val = (dis(gen) % mask) + 1; // avoid zero
            context[var.first] = val;
        }
        std::vector<std::map<std::string, variant> > myContexts = {context};
        variant out = func->eval(myContexts).at(0);
        contexts.push_back(context);
        expectedOutputs.push_back(out);
    }
}

void Solver::fillExamples(ASTNode *func, const std::vector<std::pair<std::string, Helper::Type>> &vars, int n) {
    // check whether we have BitVec64 type
    if (!std::all_of(vars.begin(), vars.end(), [](const std::pair<std::string,Helper::Type>& var) {
        return var.second == Helper::Type::BitVec64;
    })) {
        return;
    }
    // create random device
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis(
        std::numeric_limits<std::uint64_t>::min(),
        std::numeric_limits<std::uint64_t>::max()
    );

    // generate examples
    generateExamplesFromFuncSpec(func, vars, n, gen, dis);
}

std::optional<std::unique_ptr<IOExample>> Solver::checkProgWithSmt(const std::string &prog, int timeout_seconds) {
    std::packaged_task<std::optional<std::unique_ptr<IOExample>>(const std::string & string)> task([this] (const std::string& str) {
        return smtProcess->checkValidity(str);
    });
    auto future = task.get_future();
    std::thread thr(std::move(task), prog);
    auto duration = std::chrono::seconds(timeout_seconds);
    if (future.wait_for(duration) != std::future_status::timeout)
    {
        thr.join();
        std::optional<std::unique_ptr<IOExample> > out = future.get();
        return out;
    }
    thr.detach(); // we leave the thread still running -- this is no problem because we terminate the app shortly anyway
    return std::nullopt;
}

void Solver::solvePbe() {
    if(contexts.size() > maxContexts) {
        solvePBEWithExampleSampling();
    }
    else{
        initEnumerator();
        solveWithCurrentExamples();
    }
}

void Solver::printDuration(std::chrono::milliseconds duration) {
    long long milliseconds = duration.count();
    long long seconds = milliseconds / 1000;
    long long milliseconds_rest = milliseconds % 1000;
    long long minutes = seconds / 60;
    long long seconds_rest = seconds % 60;
    BOOST_LOG_TRIVIAL(info) << "Duration: " << minutes << " minutes, " << seconds_rest << " seconds, and " << milliseconds_rest << " milliseconds";
}

void Solver::solveCegis() {
    bool solved = false;
    while (!solved) {
        BOOST_LOG_TRIVIAL(info) << "Invoking SMT Solver";
        std::optional<std::unique_ptr<IOExample> > example = checkProgWithSmt(solution.value()->getCode());
        if (!example.has_value()) solved = true;
        else {
            BOOST_LOG_TRIVIAL(info) << "Intermediate Solution: " << solution.value()->getCode();
            BOOST_LOG_TRIVIAL(info) << "Intermediate Solution Size: " << solution.value()->getTerms();
            BOOST_LOG_TRIVIAL(info) << "New Example: " << example->get()->toString();
            expectedOutputs.push_back(example.value()->getOutput());
            contexts.push_back(example.value()->getInput());
            enumerator->addExample(example.value()->getInput());
            solveWithCurrentExamples();
        }
    }
}

void Solver::solveFuncApp() {
    std::string funcAppName = syGuSFileTask.getConstrFuncName();
    ASTNode* func = syGuSFileTask.getDefineFuncsMap().at(funcAppName);
    if (func != nullptr) {
        std::vector<std::pair<std::string, Helper::Type> > vars = syGuSFileTask.getFunctionParameters();
        fillExamples(func, vars, 80); // generate 80 Examples from spec
        delete func;
        solvePBEWithExampleSampling(); // solve with generated examples
    }
    // now do classic cegis
    solveCegis();
}

void Solver::solveClassicCegis() {
    // classic cegis
    initEnumerator();
    solveWithCurrentExamples();
    solveCegis();
}

std::optional<ASTNode*> Solver::solve() {
    auto start = std::chrono::high_resolution_clock::now();
    BOOST_LOG_TRIVIAL(info) << "Start Solving...";
    if(syGuSFileTask.isPbe()) {
        solvePbe();
    }
    else if (syGuSFileTask.isIsFuncAppConstrType()) {
        solveFuncApp();
    }
    else {
        solveClassicCegis();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    BOOST_LOG_TRIVIAL(info) << "Finished Solving!";
    if (!(enumMode == Helper::EnumerationMode::SizePortfolioBV || enumMode == Helper::EnumerationMode::SizePortfolioString)) {
        BOOST_LOG_TRIVIAL(info) << "Enumerated " << enumerator->getTotalProgsEnumerated() << " programs";
    }
    printDuration(duration);
    if (solution.has_value()) {
        printSolution();
        exit(0); //lets the operating system clear memory
    }
    return solution;
}

void Solver::printInfo() {
    BOOST_LOG_TRIVIAL(info)
     << "Solver: " << magic_enum::enum_name(enumMode)  << "; "
     << "SMT Backend: " << magic_enum::enum_name(smtSolver) << "; "
     << "Benchmark Name: " << filenameWithoutPath;
    Helper::Logic logic = syGuSFileTask.getLogic();
    const std::string& functionName = syGuSFileTask.getFunctionName();
    Helper::Type functionReturnType = syGuSFileTask.getFunctionReturnType();
    bool isPbe = syGuSFileTask.isPbe();

    BOOST_LOG_TRIVIAL(info)
     << "Logic: " << magic_enum::enum_name(logic) << "; "
     << "Function Name: " << functionName << "; "
     << "Function Return Type: " << magic_enum::enum_name(functionReturnType) << "; "
     << "Is PBE: " << std::boolalpha << isPbe << "; "
     << "ITE Disabled: " << std::boolalpha << disableIte;

    std::vector<std::shared_ptr<IOExample>> examples = syGuSFileTask.getExamples();
    if(!examples.empty()) {
        BOOST_LOG_TRIVIAL(info) << "Examples:";
        for(const std::shared_ptr<IOExample>& ex : examples) {
            BOOST_LOG_TRIVIAL(info) << ex->toString();
        }
    }
}

std::string getFileNameWithoutPath(const std::string& filename) {
    std::regex pattern("^(?:.*\\/)?([^\\/]+?|)(?=(?:\\.[^\\/.]*)?$)");
    std::smatch match;
    if (std::regex_search(filename, match, pattern)) {
        // Check if the group is matched
        if (match.size() > 1) {
            return match[1].str();
        }
    }
    return "";
}

Solver::Solver(const std::string& filename, Helper::EnumerationMode enumMode, Helper::SMTSolver smtSolver, bool disableIte) : filename(filename), syGuSFileTask(filename), enumMode(enumMode), smtSolver(smtSolver), disableIte(disableIte) {
    oeManager = std::make_shared<OEValuesManager>();
    contexts = syGuSFileTask.getContexts();
    expectedOutputs = syGuSFileTask.getExpectedValues();
    vocabFactory = syGuSFileTask.getVocabFactory();
    filenameWithoutPath = getFileNameWithoutPath(filename);
    maxSize = Config::GetInstance()->getMaxSize();
    maxContexts = Config::GetInstance()->getMaxContexts();

    if(disableIte) {
        vocabFactory->moveIteOperatorsOutOfNonLeaves();
        vocabFactory->moveBoolOperatorsOutOfNonLeaves();
    }
    if(!syGuSFileTask.isPbe()) {
        if(smtSolver == Helper::SMTSolver::Z3) smtProcess = std::make_shared<Z3SMTProcess>(&syGuSFileTask);
        //else if(smtSolver == Helper::SMTSolver::CVC5) smtProcess = std::make_shared<CVC5SMTProcess>(&syGuSFileTask);
        else
            BOOST_LOG_TRIVIAL(fatal) << "Incorrect Backend SMT Solver Specification";
    }
}

bool Solver::isSizeBasedAndNotConcurrent() {
    return enumMode == Helper::EnumerationMode::Size || enumMode ==
           Helper::EnumerationMode::SizeBottomUpXorAdd || enumMode ==
           Helper::EnumerationMode::SizeBottomUpAndUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBottomUpOrUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBottomUpMulUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBottomUpHDUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBottomUpXorAddHDUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBasedCombinedBottomUp
           || enumMode == Helper::EnumerationMode::SizeBottomUpConcatUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox
           || enumMode == Helper::EnumerationMode::SizeBottomUpReplaceUnderApprox;
}

void Solver::solveWithCurrentExamples() {
    BOOST_LOG_TRIVIAL(info) << "Current Examples:";
    for (int i = 0; i < contexts.size(); i++) {
        BOOST_LOG_TRIVIAL(info) << Helper::contextToString(contexts[i]) << " -> " << Helper::variantToString(expectedOutputs[i]);
    }
    oeManager->clear();
    int lastSize = 0;
    int lastTotalProgsEnumerated = 0;
    while(enumerator->hasNext()) {
        ASTNode* prog = enumerator->next();
        if (prog->getTerms() != lastSize && isSizeBasedAndNotConcurrent()) {
            BOOST_LOG_TRIVIAL(info) << "Finished Size: " << lastSize;
            lastSize = prog->getTerms();
            BOOST_LOG_TRIVIAL(info) << "Total Progs Enumerated: " << enumerator->getTotalProgsEnumerated() - 1;
            BOOST_LOG_TRIVIAL(info) << "New Progs Enumerated: " << enumerator->getTotalProgsEnumerated() - 1 - lastTotalProgsEnumerated;
            lastTotalProgsEnumerated = enumerator->getTotalProgsEnumerated() - 1;
            if (prog->getTerms() > maxSize) return;
        }
        if(prog->getValues() == expectedOutputs) {
            BOOST_LOG_TRIVIAL(info) << "Total Progs Enumerated: " << enumerator->getTotalProgsEnumerated();
            solution = prog;
            return;
        }
    }
}

void Solver::printSolution() {
    if(solution.has_value()) {
        BOOST_LOG_TRIVIAL(info) << "Solution: " << solution.value()->getCode();
        BOOST_LOG_TRIVIAL(info) << "Real Solution Size: " << solution.value()->getRealSize();
        BOOST_LOG_TRIVIAL(info) << "Fake Solution Size: " << solution.value()->getTerms();
        BOOST_LOG_TRIVIAL(info) << "Solution Cost: " << solution.value()->getCost();
        BOOST_LOG_TRIVIAL(info) << "Num Examples: " << contexts.size();
    }
    else {
        BOOST_LOG_TRIVIAL(warning) << "There currently is no solution!";
    }
    std::cout << solution.value()->getCode() << std::endl;
}

void Solver::backupExamples(std::vector<std::map<std::string, variant>>& contextsBackup, std::vector<variant>& expectedOutputsBackup) {
    for(const auto& context : contexts) {
        contextsBackup.push_back(context);
    }
    expectedOutputsBackup.reserve(expectedOutputs.size());
    for (const auto &expectedOutput: expectedOutputs) {
        expectedOutputsBackup.push_back(expectedOutput);
    }
    contexts.clear();
    expectedOutputs.clear();
}

void Solver::sampleExamples(const std::vector<std::map<std::string, variant>>& contextsBackup, const std::vector<variant>& expectedOutputsBackup) {
    bool isExample = Config::GetInstance()->isRunExample();
    std::vector<int> macRands = {520932930, 16807, 33614, 50421, 67228, 84035, 100842, 117649, 134456, 151263}; // mimic mac os random numbers for replicating results more accurately
    int seed = 0;
    // sample contexts
    std::set<int> contextIndices;
    while (contextIndices.size() < (maxContexts < contextsBackup.size() ? maxContexts : contextsBackup.size())) {
        std::srand(seed);
        int myRand = seed < macRands.size() ? macRands[seed] : std::rand();
        int random_value = !isExample ? myRand % contextsBackup.size() : 0;
        contextIndices.insert(random_value);
        seed++;
    }
    for (int i: contextIndices) {
        contexts.push_back(contextsBackup.at(i));
        expectedOutputs.push_back(expectedOutputsBackup.at(i));
    }
}

void Solver::updateEnumOrder(const std::vector<std::map<std::string, variant>>& contextsBackup, const std::vector<variant>& expectedOutputsBackup) {
    // add useful Vocabmakers
    if(Config::GetInstance()->isReuseComponents()) {
        std::vector<std::shared_ptr<VocabMaker>> makers = solution.value()->getAndMaximalComponentsMakers(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(makers, Helper::EnumerationMode::SizeBottomUpAndUnderApprox);
        std::vector<std::shared_ptr<VocabMaker>> makersOr = solution.value()->getOrMaximalComponentsMakers(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(makersOr, Helper::EnumerationMode::SizeBottomUpOrUnderApprox);
        std::vector<std::shared_ptr<VocabMaker>> usefulReplacements = solution.value()->getUsefulReplacements(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(usefulReplacements, Helper::EnumerationMode::SizeBottomUpReplaceUnderApprox);
        std::vector<std::shared_ptr<VocabMaker>> usefulSubstrings = solution.value()->getUsefulSubstrings(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(usefulSubstrings, Helper::EnumerationMode::SizeBottomUpConcatUnderApprox);
        std::vector<std::shared_ptr<VocabMaker>> usefulSuperStrings = solution.value()->getUsefulSuperstrings(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(usefulSuperStrings, Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox);
        std::vector<std::shared_ptr<VocabMaker>> usefulFactors = solution.value()->getUseFulFactors(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(usefulFactors, Helper::EnumerationMode::SizeBottomUpMulUnderApprox);
        std::vector<std::shared_ptr<VocabMaker>> closeProgs = solution.value()->getCloseProgs(contextsBackup, expectedOutputsBackup);
        vocabFactory->addLeaveOperators(closeProgs, Helper::EnumerationMode::SizeBottomUpHDUnderApprox);
    }
}

bool Solver::checkValidityAndRefine(const std::vector<std::map<std::string, variant>>& contextsBackup, const std::vector<variant>& expectedOutputsBackup, const std::vector<variant>& output) {
    int numWrongOutputs = 0;
    bool isWrong = false;
    for(int i = 0; i < output.size(); i++) { // check validity
        if(output.at(i) == expectedOutputsBackup.at(i)) continue;
        isWrong = true;
        if (numWrongOutputs < maxContexts) {
            contexts.push_back(contextsBackup.at(i));
            expectedOutputs.push_back(expectedOutputsBackup.at(i));
        }
        numWrongOutputs++;
    }
    return isWrong;
}

void Solver::solvePBEWithExampleSampling() {
    std::vector<std::map<std::string, variant>> contextsBackup;
    std::vector<variant> expectedOutputsBackup;
    backupExamples(contextsBackup, expectedOutputsBackup);
    sampleExamples(contextsBackup, expectedOutputsBackup);

    initEnumerator();
    while(!solution.has_value()){
        enumerator->resetEnumeration();
        solveWithCurrentExamples();
        if(!solution.has_value()) return; // unrealizable
        std::vector<variant> output = solution.value()->eval(contextsBackup); // get output on all examples
        bool isWrong = checkValidityAndRefine(contextsBackup, expectedOutputsBackup, output);
        if(isWrong) {
            updateEnumOrder(contextsBackup, expectedOutputsBackup);
            solution.reset();
        }
    }
}

void Solver::initEnumerator() {
    if (enumMode == Helper::EnumerationMode::Size) {
        enumerator = std::make_shared<SizeBasedEnumerator>(oeManager, vocabFactory, contexts);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpXorAdd) {
        enumerator = std::make_shared<SizeBasedEnumeratorXorAdd>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpAndUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumeratorAndUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpOrUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumeratorOrUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpMulUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumeratorMulUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpHDUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumeratorHDUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpXorAddHDUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumerationXorAddHDUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpConcatUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumerationConcatUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpSubstrUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumerationSubstrUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBottomUpReplaceUnderApprox) {
        enumerator = std::make_shared<SizeBasedEnumerationReplaceUnderApprox>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBasedCombinedBottomUp) {
        enumerator = std::make_shared<SizeBasedCombinedBottomUp>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizeBasedCombinedBottomUpStrings) {
        enumerator = std::make_shared<SizeBasedCombinedBottomUpStrings>(oeManager, vocabFactory, contexts, expectedOutputs);
    }
    else if(enumMode == Helper::EnumerationMode::SizePortfolioBV) {
        enumerator = std::make_shared<SizePortfolioBV>(contexts, oeManager, expectedOutputs, vocabFactory);
    }
    else if(enumMode == Helper::EnumerationMode::SizePortfolioString) {
        enumerator = std::make_shared<SizePortfolioString>(contexts, oeManager, expectedOutputs, vocabFactory);
    }
    else {
        assert(false && "invalid mode");
    }
}
