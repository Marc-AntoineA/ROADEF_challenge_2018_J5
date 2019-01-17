#include "kPermutation.h"

#include "../../GlassKernel/heuristic.h"

#include <string>
#include <algorithm>

#define NB_SWAP 5

// K-firsts indexes of permutation are corresponding to the permutation 
// pi[0] go to pi[1].. and pi[k - 1] go to pi[0]

KPermutation::KPermutation(Heuristic* heuristic, unsigned int k)
    : GlassMove(heuristic), k(k) {
    this->name = std::to_string(k) + "_permutation";
    initPermutation();
} 

void KPermutation::initPermutation() {
    permutation.clear();
    for (unsigned int index = 0; index < getSequenceSize(); index++) {
        permutation.push_back(index);
    }
}

void KPermutation::mixPermutation() {
    unsigned int n = getSequenceSize();
    for (unsigned int swapIndex = 0; swapIndex < NB_SWAP; swapIndex++) {
        unsigned int i = heuristic->glassRandint(0, n);
        unsigned int j = heuristic->glassRandint(0, n);
        if (i == j) continue;
        unsigned int tmp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = tmp;
    }
}

void KPermutation::displayPermutation() {
    std::cout << "Permutation : ";
    for (unsigned int index = 0; index < getSequenceSize(); index++) {
        std::cout << permutation[index] << " ";
    }
    std::cout << std::endl;
}

bool KPermutation::doNothing() {
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int firstStack = sequence[permutation[0]];
    for (unsigned int index = 1; index < k; index++) {
        if (sequence[permutation[index]] != firstStack)
            return false;
    }
    return true;
}

void KPermutation::applyPermutation() {
    assert(k > 0);
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int tmp = sequence[permutation[0]];
    for (unsigned int index = 1; index < k; index++) {
        sequence[permutation[index - 1]] = sequence[permutation[index]];
    }
    sequence[permutation[k - 1]] = tmp;
}

void KPermutation::revertPermutation() {
    assert(k > 0);
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    unsigned int tmp = sequence[permutation[k - 1]];
    for (unsigned int index = k - 1; index > 0; index--) {
        sequence[permutation[index]] = sequence[permutation[index - 1]];
    }
    sequence[permutation[0]] = tmp;
}

int KPermutation::attempt() {
    addTested();
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    if(k >= sequence.size()) std::cout << k << " & " << sequence.size() << std::endl;
    assert(k < sequence.size());
    if (doNothing()) return NOTHING;
    mixPermutation();
    applyPermutation();
    unsigned int minIndex = permutation[0];
    for (unsigned int index = 0; index < k; index++) {
        minIndex = std::min(minIndex, permutation[index]);
    }
    return minIndex;
}

void KPermutation::commit() {

}

void KPermutation::revert() {
    revertPermutation();
}
