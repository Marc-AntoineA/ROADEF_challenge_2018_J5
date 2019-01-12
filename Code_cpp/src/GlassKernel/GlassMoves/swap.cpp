#include "swap.h"

#include <cstdlib>
#include "../../GlassKernel/heuristic.h"

#include <vector>
#include <iostream>

bool Swap::attempt() {
    addTested();
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    firstIndex = heuristic->glassRandint(0, sequence.size());
    lastIndex = heuristic->glassRandint(0, sequence.size());
    if (firstIndex == lastIndex) return false;
    if (sequence[firstIndex] == sequence[lastIndex]) return false;
    unsigned int tmp = sequence[firstIndex];
    sequence[firstIndex] = sequence[lastIndex];
    sequence[lastIndex] = tmp;
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