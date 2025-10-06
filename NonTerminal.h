
#ifndef NONTERMINAL_H
#define NONTERMINAL_H

#include <string>

#include "Helper.h"


class NonTerminal {
    /**
     * name of nonterminal
     */
    std::string name;
    /**
     * data type of nonterminal
     */
    Helper::Type type;
    /**
     * equality operator
     * @param lhs
     * @param rhs
     * @return true, iff name is equal
     */
    friend bool operator==(const NonTerminal &lhs, const NonTerminal &rhs) {
        return lhs.name == rhs.name;
    }

    friend bool operator!=(const NonTerminal &lhs, const NonTerminal &rhs) {
        return !(lhs == rhs);
    }

public:
    /**
     *
     * @return data type corresponding to nonterminal
     */
    [[nodiscard]] Helper::Type getType() const;

    /**
     *
     * @return name of nonterminal
     */
    [[nodiscard]] std::string getName() const;

    /**
     *
     * @param name name of nonterm
     * @param type return type of nonterm
     */
    explicit NonTerminal(const std::string &name, Helper::Type type)
        : name(name), type(type) {
    }
};


namespace std {
    template <>
    struct hash<NonTerminal> {
        auto operator()(const NonTerminal &xyz) const -> size_t {
            return hash<std::string>{}(xyz.getName());
        }
    };
}


#endif //NONTERMINAL_H
