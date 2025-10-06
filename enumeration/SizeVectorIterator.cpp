
#include "SizeVectorIterator.h"

#include <utility>
#include "cassert"

SizeVectorIterator::SizeVectorIterator(const std::vector<std::vector<int>>& sizeLists, int targetSize, int length) : targetSize(targetSize), sizeLists(sizeLists), length(length) {
    // initialize childrenLists
    for(int i = 0; i < length; i++) {
        sizeListsIndices.push_back(0);
    }
    // set first element
    nextChild.reset();
    bool found = false;
    std::vector<int> out;
    out.reserve(sizeListsIndices.size());
    int sum = 0;
    for(int i = 0; i < sizeListsIndices.size(); i++) {
        int index = sizeListsIndices.at(i);
        int curr = sizeLists.at(i).at(index);
        out.push_back(curr);
        sum += curr;
    }
    if(sum == targetSize) nextChild = out;
    else setNextChild();
}

std::optional<std::vector<int>> SizeVectorIterator::getNext() {
    std::optional<std::vector<int>> out;
    if(!nextChild.has_value()) {
        if(!setNextChild()){
            return out;
        }
    }
    assert(nextChild.has_value());
    out = nextChild.value();
    nextChild.reset();
    return out;
}

bool SizeVectorIterator::setNextChild() {
    nextChild.reset();
    while(!nextChild.has_value()) {
        if(!advance()) return false;

        std::vector<int> out;
        int sum = 0;
        for(int i = 0; i < sizeListsIndices.size(); i++) {
            int index = sizeListsIndices.at(i);
            int curr = sizeLists.at(i).at(index);
            out.push_back(curr);
            sum += curr;
        }
        if(sum == targetSize) nextChild = out;
    }
    return true;
}

bool SizeVectorIterator::hasNext() {
    if(nextChild.has_value()) return true;
    else return setNextChild();
}

bool SizeVectorIterator::advance() {
    for(int i = 0; i < sizeListsIndices.size(); i++) {
        std::vector<int> currList = sizeLists.at(i);
        int index = sizeListsIndices.at(i);
        if(index == currList.size() - 1) continue; // already at last element
        // else
        sizeListsIndices[i] = index + 1;
        // reset previous indices
        for(int j = 0; j < i; j++) {
            sizeListsIndices[j] = 0;
        }
        return true;
    }
    return false;
}
