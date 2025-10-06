
#ifndef MYSOLVER_SIZEBASEDCHILDRENITERATOR_H
#define MYSOLVER_SIZEBASEDCHILDRENITERATOR_H

#include "../ast/ASTNode.h"
#include "SizeVectorIterator.h"
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"

template<class T, class U = NonTerminal>
class SizeBasedChildrenIterator {
    /**
     * type of each child
     */
    std::vector<U> childTypes;

    /**
     * target size
     */
    int size;

    /**
     * current program pool
     */
    const boost::unordered_map<int, std::vector<T>>& childrenBank;

    /**
     * current children sizes
     */
    std::vector<int> sizeVector;

    /**
     * size vector iterator to generate new size vectors
     */
    std::unique_ptr<SizeVectorIterator> sizeVectorIterator;

    /**
     * available programs for current size vector and current child types
     */
    std::vector<std::vector<T>> childrenLists;

    /**
     * current indices for iteration
     */
    std::vector<int> childrenListsIndices;

    /**
     * contains next child (vector)
     */
    std::optional<std::vector<T>> nextChild;

    /**
     * tries to advance the childrenListsIndices.
     * if that is not possible, gets next size vector
     * if that is not possible, returns false
     * @return true if advancing succeded
     */
    bool advance();

    /**
     * sets next child (vector)
     * @return true, iff there is a next child
     */
    bool setNextChild();

    /**
     * fills children lists for the current size vector
     * i.e. if the size  vector is <1,2>
     * then childrenLists[0] contains all programs of size 1 with type childTypes[0]
     * and childrenLists[1] contains all programs of size 2 with type childTypes[1]
     * also sets childrenListsIndices to 0
     * @return true iff children lists have been set
     */
    bool setChildrenLists();

    /**
     * checks whether the given program is of a given type
     * @param child program to check
     * @param type type to check
     * @return child.type == type
     */
    bool isOfType(T child, const U &type);

    /**
     * fills vector which contains the avaibale sizes for each child index
     * @return available sizes for each child slot
     */
    std::vector<std::vector<int>> fillSizeVectorLists(const boost::unordered_map<int, std::vector<T>> &childrenBank, int size);

    /**
     * is true when there is no size (i.e.) no program available for a child index
     */
    bool nonAvailable = false;

    /**
     * is true when current maker is commutative
     */
    bool isCommutative = false;

public:
    /**
    *
    * @param childrenBank current program pool
    * @param childTypes types for children
    * @param size target program size
    */
    SizeBasedChildrenIterator<T, U>(const boost::unordered_map<int, std::vector<T>> &childrenBank,
                     const std::vector<U> &childTypes, int size);

    /**
    *
    * @param childrenBank current program pool
    * @param childTypes types for children
    * @param size target program size
    * @param isCommutative whether the current maker is commutative
    */
    SizeBasedChildrenIterator<T, U>(const boost::unordered_map<int, std::vector<T>> &childrenBank,
                     const std::vector<U> &childTypes, int size, bool isCommutative);
    /**
     *
     * @return true, iff there is a next child (vector)
     */
    bool hasNext();

    /**
     *
     * @return next child (vector)
     */
    std::vector<T> getNext();
};


template<class T, class U>
std::vector<std::vector<int>> SizeBasedChildrenIterator<T, U>::fillSizeVectorLists(const boost::unordered_map<int, std::vector<T>> &childrenBank, int size) {
    std::vector<std::vector<int>> out;
    out.reserve(childTypes.size());
    for (const U& childType : childTypes) {
        std::vector<int> sizesForCurrentType;
        for (int i = 0; i <= size; i++) {
            auto it = childrenBank.find(i);
            if (it != childrenBank.end()) {
                for (const T &prog: it->second) {
                    if (isOfType(prog, childType)) {
                        sizesForCurrentType.push_back(i);
                        break;
                    }
                }
            }
        }
        if(sizesForCurrentType.empty()) {
            nonAvailable = true;
            return out;
        }
        out.push_back(std::move(sizesForCurrentType));
    }
    return out;
}



template<class T, class U>
SizeBasedChildrenIterator<T, U>::SizeBasedChildrenIterator(
    const boost::unordered_map<int, std::vector<T>> &childrenBank, const std::vector<U> &childTypes, int size,
    bool isCommutative) : childTypes(childTypes), size(size), childrenBank(childrenBank), isCommutative(isCommutative) {
    // initialize childrenLists
    const std::vector<std::vector<int>>& mySizeVectorLists = fillSizeVectorLists(childrenBank, size);
    if(nonAvailable) return;
    sizeVectorIterator = std::make_unique<SizeVectorIterator>(mySizeVectorLists, size, childTypes.size());

    // set first element
    bool foundChildList = false;
    while(sizeVectorIterator->hasNext()) {
        sizeVector = sizeVectorIterator->getNext().value();
        if(setChildrenLists()) {
            foundChildList = true;
            break;
        }
    }

    nextChild.reset();
    if(!foundChildList) {
        return;
    }
    std::vector<T> out;


    out.reserve(childrenListsIndices.size());
    for(int i = 0; i < childrenListsIndices.size(); i++) {
        int index = childrenListsIndices.at(i);
        T curr = childrenLists.at(i).at(index);
        out.push_back(curr);
    }
    nextChild = out;

    if(size == 0 && !foundChildList) { // vars and literals
        assert(childrenBank.empty() && "only works without children");
    }
}

template<class T, class U>
SizeBasedChildrenIterator<T, U>::SizeBasedChildrenIterator(const boost::unordered_map<int, std::vector<T>> &childrenBank,
                                                        const std::vector<U> &childTypes, int size) : SizeBasedChildrenIterator(childrenBank, childTypes, size, false){};

template<class T, class U>
bool SizeBasedChildrenIterator<T, U>::advance() {
    for(int i = 0; i < childrenLists.size(); i++) {
        const std::vector<T>& currList = childrenLists.at(i);
        int index = childrenListsIndices.at(i);
        if(index == currList.size() - 1) continue; // already at last element
        // else
        childrenListsIndices[i] = index + 1;
        // reset previous indices
        for(int j = 0; j < i; j++) {
            childrenListsIndices[j] = 0;
        }
        return true;
    }
    // we cannot advance with current size vector, get next
    while(sizeVectorIterator->hasNext()) {
        sizeVector = sizeVectorIterator->getNext().value();
        if(isCommutative && !std::is_sorted(sizeVector.begin(), sizeVector.end())) {
            continue;
        }
        if(setChildrenLists()) return true;
    }

    return false;
}

template<class T, class U>
bool SizeBasedChildrenIterator<T, U>::setNextChild() {
    if(childrenListsIndices.size() != childTypes.size()) return false;
    nextChild.reset();
    while(!nextChild.has_value()) {
        if(!advance()) return false;

        std::vector<T> out;
        out.reserve(childrenListsIndices.size());
        for(int i = 0; i < childrenListsIndices.size(); i++) {
            int index = childrenListsIndices.at(i);
            const T& curr = childrenLists.at(i).at(index);
            out.push_back(curr);
        }
        nextChild = out;
    }
    return true;
}

template<class T, class U>
bool SizeBasedChildrenIterator<T, U>::hasNext() {
    if(nonAvailable) return false;
    if(nextChild.has_value()) return true;
    else return setNextChild();
}

template<class T, class U>
std::vector<T> SizeBasedChildrenIterator<T, U>::getNext() {
    if(!nextChild.has_value()) setNextChild();
    assert(nextChild.has_value());
    std::vector<T> out = std::move(nextChild.value()); // langsam TODO
    nextChild.reset();
    return out;
}

template<class T, class U>
bool SizeBasedChildrenIterator<T, U>::setChildrenLists() {
    childrenLists.clear();
    childrenListsIndices.clear();
    for(int i = 0; i < sizeVector.size(); i++) {
        int currSize = sizeVector.at(i);
        try{
            const std::vector<T>& potentialChildren = childrenBank.at(currSize);
            std::vector<T> children;
            for(T potentialChild : potentialChildren) {
                if(isOfType(potentialChild, childTypes.at(i))) {
                    children.push_back(potentialChild);
                }
            }
            if(children.empty()) {
                // no child of given type for given size
                return false;
            }
            childrenLists.push_back(children);
            childrenListsIndices.push_back(0);
        }
        catch (const std::out_of_range& ex) {
            // size not available
            return false;
        }
    }
    return true;
}

#endif //MYSOLVER_SIZEBASEDCHILDRENITERATOR_H
