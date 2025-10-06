
#ifndef MYSOLVER_SOLVER_H
#define MYSOLVER_SOLVER_H
#include <random>

#include "string"
#include "SyGuSFileTask.h"
#include "enumeration/OEValuesManager.h"
#include "Helper.h"
#include "SMTProcesses/SMTProcess.h"
#include "enumeration/SizeBasedEnumerator.h"

class Solver {
    /**
     * filename of the sygus-file that is solved
     */
    std::string filename;
    /**
     * file name without the whole path for better readability
     */
    std::string filenameWithoutPath;
    /**
     * parser interface
     */
    SyGuSFileTask syGuSFileTask;
    /**
     * Observational Equivalence Manager. Handles Factorization with OE
     */
    std::shared_ptr<OEValuesManager> oeManager;
    /**
     * variable assignments / input examples
     */
    std::vector<std::map<std::string, variant>> contexts;
    /**
     * output examples
     */
    std::vector<variant> expectedOutputs;
    /**
     * enumerator to solve the synthesis task with
     */
    std::shared_ptr<Enumerator> enumerator;
    /**
     * which enumerator to choose, i.e. size based, size based with pruning, etc.
     */
    Helper::EnumerationMode enumMode;

    /**
     * Solves PBE task without considering all examples at once --> abstraction
     */
    void solvePBEWithExampleSampling();

    /**
     * solves task using the currently selected examples
     */
    void solveWithCurrentExamples();

    /**
     * initializes the enumerator
     */
    void initEnumerator();

    /**
     * vocabFactory --> grammar
     */
    std::shared_ptr<VocabFactory> vocabFactory;

    /**
     * field for output
     */
    std::optional<ASTNode *> solution = std::nullopt;

    /**
     * SMT solver to use for the CEGIS backend
     */
    Helper::SMTSolver smtSolver;

    /**
     * connection to smt solver
     */
    std::shared_ptr<SMTProcess> smtProcess;

    /**
     * whether ite should be disabled
     */
    bool disableIte;

    /**
     * how many contexts should be in the initial abstraction
     */
    int maxContexts;

    /**
     * maximal program size
     */
    int maxSize;

    /**
     * Generates n examples for given specification function
     * @param func ASTNode representing the specification function
     * @param vars variables inside the specification function
     * @param i number of examples to generate
     */
    void fillExamples(ASTNode * func, const std::vector<std::pair<std::string, Helper::Type>> & vars, int i);

    /**
     * checks solution candidate with smt solver; uses timeout, if the timeout elapsed without the smt
     * solver yielding a counter-example, the solution is considered solved
     * @param prog solution candidate
     * @param timeout_seconds how long the timeout should be
     * @return optional: if the optional has a value, it is a counter-example, if it does not have a value, there is no
     * counter example
     */
    std::optional<std::unique_ptr<IOExample>> checkProgWithSmt(const std::string& prog, int timeout_seconds = 10);

    /**
     * solves Programming By Example synthesis task
     */
    void solvePbe();

    /**
     * prints duration in readable format, i.e. Minutes, Seconds, Milliseconds
     * @param duration duration to print
     */
    static void printDuration(std::chrono::milliseconds duration);

    /**
     * solves benchmark with cegis loop
     */
    void solveCegis();

    /**
     * if cegis specification is a function application, generate examples beforehand and solve with cegis then.
     * this is useful for Hacker's Delight benchmarks
     */
    void solveFuncApp();

    /**
     * solves classic cegis problem
     */
    void solveClassicCegis();

    /**
     * backs up examples
     * @param contextsBackup object to save contexts in
     * @param expectedOutputsBackup object to save expected outputs in
     */
    void backupExamples(std::vector<std::map<std::string, variant>>& contextsBackup, std::vector<variant>& expectedOutputsBackup);

    /**
     * samples examples from contextsbackup and expectedoutputs backup
     * @param contextsBackup all input examples
     * @param expectedOutputsBackup all output examples
     */
    void sampleExamples(const std::vector<std::map<std::string, variant>>& contextsBackup, const std::vector<variant>& expectedOutputsBackup);

    /**
     * learns from wrong program by updating the enumeration order
     * @param contextsBackup all input examples
     * @param expectedOutputsBackup all output examples
     */
    void updateEnumOrder(const std::vector<std::map<std::string, variant>>& contextsBackup, const std::vector<variant>& expectedOutputsBackup);

    /**
     * checks validity of current program and refines abstraction
     * by adding new examples to current examples
     * @param contextsBackup all input examples
     * @param expectedOutputsBackup all output examples
     * @param output output of the current program
     * @return true, when current program is wrong / spurious
     */
    bool checkValidityAndRefine(const std::vector<std::map<std::string, variant>>& contextsBackup, const std::vector<variant>& expectedOutputsBackup, const std::vector<variant>& output);

    /**
     * generates examples from functional specification. Useful for Hacker's Delight benchmarks.
     * @param func program representing functional specification
     * @param vars variables of func specification
     * @param n number of examples to generate
     * @param gen random number generator
     * @param dis random distribution
     */
    void generateExamplesFromFuncSpec(ASTNode *func, const std::vector<std::pair<std::string, Helper::Type>> &vars, int n, std::mt19937 gen, std::uniform_int_distribution<unsigned long long> dis);

    /**
     *
     * @return whether enumeration mode is size based and not concurrent
     */
    bool isSizeBasedAndNotConcurrent();

public:
    /**
     * solves the given SyGuS problem
     * @return solution to the SyGuS problem, or nullopt if we could not find one
     */
    std::optional<ASTNode*> solve();

    /**
     * prints solver info, i.e. settings used, logic used, filename etc.
     */
    void printInfo();

    /**
     * Prints solution and information about the solution
     */
    void printSolution();

    /**
     * construct solver
     * @param filename path to sygus-file which to solve
     * @param enumMode solver strategy
     * @param smtSolver which smt backend to use (Z3 or CVC5)
     * @param disableIte whether to disable ITE constructs (default = false)
     */
    explicit Solver(const std::string& filename, Helper::EnumerationMode enumMode, Helper::SMTSolver smtSolver, bool disableIte = false);
};


#endif //MYSOLVER_SOLVER_H
