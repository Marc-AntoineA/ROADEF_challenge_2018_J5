#include "swap.h"

#include <cstdlib>
#include "../../GlassKernel/heuristic.h"

#include <vector>
#include <iostream>
#include <algorithm>

void Swap::attempt(unsigned int& beginIndex, unsigned int& endIndex) {
    addTested();
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    firstIndex = heuristic->glassRandint(0, sequence.size() - 1);
    lastIndex = heuristic->glassRandint(firstIndex + 1, sequence.size());
    if (sequence[firstIndex] == sequence[lastIndex]) {
        beginIndex = 0;
        endIndex = 0;
        return;
    }
    std::swap(sequence[firstIndex], sequence[lastIndex]);
    beginIndex = firstIndex;
    endIndex = lastIndex;
}

void Swap::commit() {
    return;
}

void Swap::revert() {
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    std::swap(sequence[firstIndex], sequence[lastIndex]);
}