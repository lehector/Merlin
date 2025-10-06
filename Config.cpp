
#include "Config.h"

Config* Config::config_ = nullptr;

int Config::getAndOrRadius() const {
    return andOrRadius;
}

void Config::setAndOrRadius(int and_or_radius) {
    andOrRadius = and_or_radius;
}

int Config::getMulRadius() const {
    return mulRadius;
}

void Config::setMulRadius(int mul_radius) {
    mulRadius = mul_radius;
}

int Config::getHdRadius() const {
    return hdRadius;
}

void Config::setHdRadius(int hd_radius) {
    hdRadius = hd_radius;
}

int Config::getConcatSubstrRadius() const {
    return concatSubstrRadius;
}

void Config::setConcatSubstrRadius(int concat_substr_radius) {
    concatSubstrRadius = concat_substr_radius;
}

int Config::getLevenshteinRadius() const {
    return levenshteinRadius;
}

void Config::setLevenshteinRadius(int levenshtein_radius) {
    levenshteinRadius = levenshtein_radius;
}

bool Config::isReplComplete() const {
    return replComplete;
}

void Config::setReplComplete(bool repl_complete) {
    replComplete = repl_complete;
}

std::string Config::getDomain() const {
    return domain;
}

void Config::setDomain(const std::string &domain) {
    this->domain = domain;
}

int Config::getMaxContexts() const {
    return maxContexts;
}

void Config::setMaxContexts(int max_contexts) {
    maxContexts = max_contexts;
}

bool Config::isOePruning() const {
    return oePruning;
}

void Config::setOePruning(bool oe_pruning) {
    oePruning = oe_pruning;
}

bool Config::isReuseComponents() const {
    return reuseComponents;
}

void Config::setReuseComponents(bool reuse_components) {
    reuseComponents = reuse_components;
}

void Config::setFirstNValuesCheck(int first_n_values_check) {
    firstNValuesCheck = first_n_values_check;
}

int Config::getFirstNValuesCheck() const {
    return firstNValuesCheck;
}

bool Config::isRunExample() const {
    return runExample;
}

void Config::setRunExample(bool run_example) {
    runExample = run_example;
}

int Config::getMaxSize() const {
    return maxSize;
}

void Config::setMaxSize(int max_size) {
    maxSize = max_size;
}

bool Config::isSilent() const {
    return silent;
}

void Config::setSilent(bool silent) {
    this->silent = silent;
}

Config * Config::GetInstance() {
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(config_==nullptr){
        config_ = new Config();
    }
    return config_;
}

std::string Config::getFileName() const {
    return fileName;
}

void Config::setFileName(const std::string &file_name) {
    fileName = file_name;
}

bool Config::isViolation() const {
    return violation;
}

int Config::getCutoffSize() const {
    return cutoffSize;
}

void Config::setCutoffSize(int cutoff_size) {
    cutoffSize = cutoff_size;
}

void Config::setViolation(bool violation) {
    this->violation = violation;
}
