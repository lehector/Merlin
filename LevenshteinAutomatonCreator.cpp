
#include "LevenshteinAutomatonCreator.h"

mata::nfa::Nfa LevenshteinAutomatonCreator::createLevenshteinAutomaton() {
    // http://blog.notdot.net/2010/07/Damn-Cool-Algorithms-Levenshtein-Automata
    mata::nfa::Nfa nfa;
    for(int i = 0; i < term.size(); i++) {
        char c = term.at(i);
        for(int e = 0; e <= maxDistance ; e++) {
            // correct character
            nfa.delta.add(getStateNum(i, e), c, getStateNum(i+1, e));
            if(e < maxDistance) {
                // Deletion
                addTransitionForEveryChar(alphabet, i, e, i, e+1, nfa);
                // Insertion
                nfa.delta.add(getStateNum(i,e), mata::nfa::EPSILON, getStateNum(i+1,e+1));
                // substitution
                addTransitionForEveryChar(alphabet, i, e, i+1, e+1, nfa);
            }
        }
    }
    for(int e = 0; e <= maxDistance; e++) {
        if(e < maxDistance) {
            addTransitionForEveryChar(alphabet, term.size(), e, term.size(), e + 1, nfa);
        }
        nfa.final.insert(getStateNum(term.size(), e));
    }

    for(int i = 0; i <= maxDistance; i++) {
        nfa.final.insert(getStateNum(term.size()-i,i));
    }

    unsigned long initialState = getStateNum(0,0);
    nfa.initial = {initialState};
    nfa = mata::nfa::remove_epsilon(nfa); // this is necessary, otherwise the epsilon transitions are not fired
    nfa = mata::nfa::determinize(nfa);
    return nfa;
}

void LevenshteinAutomatonCreator::addTransitionForEveryChar(const std::set<char> &alphabet, int i1, int e1, int i2,
    int e2, mata::nfa::Nfa &nfa) {
    for(char c: alphabet) {
        nfa.delta.add(getStateNum(i1, e1), c, getStateNum(i2, e2));
    }
}

int LevenshteinAutomatonCreator::getStateNum(int i, int e) {
    auto it = stateLookUpMap.find({i, e});
    if(it != stateLookUpMap.end()) return it->second; // found
    int nextIndex = nextStateNum++;
    stateLookUpMap[{i, e}] = nextIndex;
    return nextIndex;
}
