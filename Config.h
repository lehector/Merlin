
#ifndef CONFIG_H
#define CONFIG_H
#include <string>


/**
 * From: https://refactoring.guru/design-patterns/singleton/cpp/example
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class Config
{

    /**
     * The Singleton's constructor should always be private to prevent direct
     * construction calls with the `new` operator.
     */
    Config() = default;

protected:
    static Config *config_;

    /**
     * filename of sygus file
     */
    std::string fileName;
    /**
     * whether to apply pruning
     */
    bool violation;
    /**
     * size threshold for pruning
     */
    int cutoffSize;
    /**
     * check for deduction:
     * before creating the complete program to check whether it is correct,
     * we only compute the first n outputs. If they are correct we compute the whole output
     */
    int firstNValuesCheck;
    /**
     * whether to learn from previous iterations
     */
    bool reuseComponents;

    /**
     * whether to apply (observational equivalence) factorization
     */
    bool oePruning;
    /**
     * whether to apply a complete version of the replace deduction
     */
    bool replComplete;
    /**
     * percentage of and/or radius
     */
    int andOrRadius;
    /**
     * percentage of mul radius
     */
    int mulRadius;
    /**
     * percentage of hd radius
     */
    int hdRadius;
    /**
     * percentage of concat and substr radius
     */
    int concatSubstrRadius;
    /**
     * total levenshtein radius
     */
    int levenshteinRadius;
    /**
     * max contexts to consider initially
     */
    int maxContexts;
    /**
     * domain of syntehsis task
     */
    std::string domain;
    /**
     * whether to print all
     */
    bool silent;

    /**
     * whether to run the example
     */
    bool runExample;

public:
    [[nodiscard]] bool isRunExample() const;

    void setRunExample(bool run_example);

    [[nodiscard]] int getMaxSize() const;

    void setMaxSize(int max_size);

protected:
    /**
     * max program size
     */
    int maxSize = 1000;

public:
    [[nodiscard]] bool isSilent() const;

    void setSilent(bool silent);

    /**
     * Singletons should not be cloneable.
     */
    Config(Config &other) = delete;


    /**
     * Singletons should not be assignable.
     */
    void operator=(const Config &) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */
    static Config *GetInstance();

    // getters and setters
    [[nodiscard]] int getAndOrRadius() const;

    void setAndOrRadius(int and_or_radius);

    [[nodiscard]] int getMulRadius() const;

    void setMulRadius(int mul_radius);

    [[nodiscard]] int getHdRadius() const;

    void setHdRadius(int hd_radius);

    [[nodiscard]] int getConcatSubstrRadius() const;

    void setConcatSubstrRadius(int concat_substr_radius);

    [[nodiscard]] int getLevenshteinRadius() const;

    void setLevenshteinRadius(int levenshtein_radius);

    [[nodiscard]] bool isReplComplete() const;

    void setReplComplete(bool repl_complete);
    [[nodiscard]] std::string getDomain() const;

    void setDomain(const std::string &domain);

    [[nodiscard]] int getMaxContexts() const;

    void setMaxContexts(int max_contexts);

    [[nodiscard]] bool isOePruning() const;

    void setOePruning(bool oe_pruning);

    [[nodiscard]] bool isReuseComponents() const;

    void setReuseComponents(bool reuse_components);

    void setFirstNValuesCheck(int first_n_values_check);

    [[nodiscard]] int getFirstNValuesCheck() const;

    [[nodiscard]] std::string getFileName() const;

    void setFileName(const std::string &file_name);

    [[nodiscard]] bool isViolation() const;

    [[nodiscard]] int getCutoffSize() const;

    void setCutoffSize(int cutoff_size);

    void setViolation(bool violation);
};
#endif //CONFIG_H
