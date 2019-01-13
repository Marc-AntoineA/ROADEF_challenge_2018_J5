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
    unsigned int tmp = sequence[firstIndex];
    sequence[firstIndex] = sequence[lastIndex];
    sequence[lastIndex] = tmp;
    return firstIndex;
}


void Swap::commit() {
    return;
}

void Swap::revert() {
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int tmp = sequence[firstIndex];
    sequence[firstIndex] = sequence[lastIndex];
    sequence[lastIndex] = tmp;
}