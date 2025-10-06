
#ifndef LEVENSHTEINAUTOMATONCREATOR_H
#define LEVENSHTEINAUTOMATONCREATOR_H
#include <set>
#include <string>
#include <mata/nfa/nfa.hh>


class LevenshteinAutomatonCreator {
    /**
     * states get ids of increasing number
     */
    int nextStateNum = 0;
    /**
     * alphabet of automaton
     */
    std::set<char> alphabet;
    /**
     * maximum distance -- radius
     */
    int maxDistance;
    /**
     * term to create automaton for
     */
    std::string term;

    /**
     *
     * @param alphabet alphabet of automaton
     * @param i1 index of state 1
     * @param e1 error count of state 1
     * @param i2 index of state 2
     * @param e2 error count of state 2
     * @param nfa nfa to add transitions to
     */
    void addTransitionForEveryChar(const std::set<char>& alphabet, int i1, int e1, int i2, int e2, mata::nfa::Nfa& nfa);

    /**
     * look up map for states
     */
    std::map<std::pair<int, int>, int> stateLookUpMap;

    /**
     *
     * @param i index of state
     * @param e error count of state
     * @return state num corresponding to i and e
     */
    int getStateNum(int i, int e);

public:
    /**
     *
     * @param alphabet alphabet of automaton
     * @param max_distance max distance to permit -- radius
     * @param term term to create automaton for
     */
    LevenshteinAutomatonCreator(const std::set<char> &alphabet, int max_distance, const std::string &term)
        : alphabet(alphabet),
          maxDistance(max_distance),
          term(term) {
    }

    /**
     *
     * @return creates levenshtein automaton
     */
    mata::nfa::Nfa createLevenshteinAutomaton();
};



#endif //LEVENSHTEINAUTOMATONCREATOR_H
