#include "kConsecutivePermutation.h"

#include "../../GlassKernel/heuristic.h"

#include <cassert>

#define NB_SWAP 5

KConsecutivePermutation::KConsecutivePermutation(Heuristic* heuristic, unsigned int k)
    : GlassMove(heuristic), k(k) {
    this->name = k + "-consecutivePermutation";
    initPermutation();
} 

void KConsecutivePermutation::initPermutation() {
    permutation.resize(k);
    for (unsigned int index = 0; index < k; index++) {
        permutation[k - index - 1] = index;
    }
}

void KConsecutivePermutation::mixPermutation() {
    for (unsigned int swapIndex = 0; swapIndex < NB_SWAP; swapIndex++) {
        unsigned int i = heuristic->glassRandint(0, k);
        unsigned int j = heuristic->glassRandint(0, k);
        if (i == j) continue;
        unsigned int tmp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = tmp;
    }
}

void KConsecutivePermutation::displayPermutation() {
    std::cout << "Permutation : ";
    for (unsigned int index = 0; index < k; index++) {
        std::cout << permutation[index] << " ";
    }
    std::cout << std::endl;
}

bool KConsecutivePermutation::doNothing() {
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int firstStack = sequence[firstIndex];
    for (unsigned int index = 1; index < k; index++) {
        if (sequence[firstIndex + index] != firstStack)
            return false;
    }
    return true;
}

void KConsecutivePermutation::applyPermutation() {
    assert(k > 0);
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int tmp = sequence[firstIndex + permutation[0]];
    for (unsigned int index = 1; index < k; index++) {
        sequence[firstIndex + permutation[index - 1]] = sequence[firstIndex + permutation[index]];
    }
    sequence[firstIndex + permutation[k - 1]] = tmp;
}

void KConsecutivePermutation::revertPermutation() {
    assert(k > 0);
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int tmp = sequence[firstIndex + permutation[k - 1]];
    for (unsigned int index = k - 1; index > 0; index--) {
        sequence[firstIndex + permutation[index]] = sequence[firstIndex + permutation[index - 1]];
    }
    sequence[firstIndex + permutation[0]] = tmp;
}

int KConsecutivePermutation::attempt() {
    addTested();
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    firstIndex = heuristic->glassRandint(0, sequence.size() - k);
    if (doNothing()) return NOTHING;
    mixPermutation();
    applyPermutation();
    return firstIndex;
}

void KConsecutivePermutation::commit() {

}

void KConsecutivePermutation::revert() {
    revertPermutation();
}
