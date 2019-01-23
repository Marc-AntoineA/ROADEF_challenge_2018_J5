#include "swap.h"

#include <cstdlib>
#include "../../GlassKernel/heuristic.h"

#include <vector>
#include <iostream>
#include <algorithm>

int Swap::attempt() {
    addTested();
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    firstIndex = heuristic->glassRandint(0, sequence.size() - 1);
    lastIndex = heuristic->glassRandint(firstIndex + 1, sequence.size());
    if (sequence[firstIndex] == sequence[lastIndex]) return NOTHING;
    std::swap(sequence[firstIndex], sequence[lastIndex]);
    return firstIndex;
}

void Swap::commit() {
    return;
}

void Swap::revert() {
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    std::swap(sequence[firstIndex], sequence[lastIndex]);
}