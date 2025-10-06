
#ifndef MYSOLVER_SIZEVECTORITERATOR_H
#define MYSOLVER_SIZEVECTORITERATOR_H

#include "vector"
#include "optional"

class SizeVectorIterator {
    /**
     * program target size, sizes in size vector will add up to this size - 1
     */
    int targetSize;

    /**
     * available sizes per child index
     */
    std::vector<std::vector<int>> sizeLists;

    /**
     * current indices of selected sizes for iteration
     */
    std::vector<int> sizeListsIndices;

    /**
     * number of sizes, i.e. length of size vector to produce
     */
    int length;

    /**
     * next size vector
     */
    std::optional<std::vector<int>> nextChild;

    /**
     * sets next size vector
     * @return true, if next size vector could be set
     */
    bool setNextChild();

    /**
     * advances the indices of the size lists
     * @return true, iff advancing was successful
     */
    bool advance();
public:
    /**
     *
     * @param sizeLists list of available sizes per index
     * @param targetSize size of target program
     * @param length number of indices, i.e. lenght of size vectors to produce
     */
    SizeVectorIterator(const std::vector<std::vector<int>>& sizeLists, int targetSize, int length);

    /**
     *
     * @return true, iff there is a next size vector
     */
    bool hasNext();

    /**
     *
     * @return next size vector
     */
    std::optional<std::vector<int>> getNext();
};


#endif //MYSOLVER_SIZEVECTORITERATOR_H
