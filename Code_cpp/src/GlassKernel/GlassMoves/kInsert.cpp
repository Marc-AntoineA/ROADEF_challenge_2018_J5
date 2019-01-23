#include "kInsert.h"

#include "../../GlassKernel/heuristic.h"

#include <iostream>
#include <string>

KInsert::KInsert(Heuristic* heuristic, unsigned int k) 
    : GlassMove(heuristic), k(k) {
    this->name = "_Insert";//
}

void KInsert::attempt(unsigned int& beginIndex, unsigned int& endIndex) {
    addTested();
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    fromIndex = heuristic->glassRandint(0, sequence.size() - 1 - k);
    toIndex = heuristic->glassRandint(0, sequence.size() - 1 - k);
    if (doNothing()) {
        beginIndex = 0;
        endIndex = 0;
        return;
    }

    std::vector<unsigned int> movedBlock;
    movedBlock.clear();
    movedBlock.insert(movedBlock.end(), sequence.begin() + fromIndex, sequence.begin() + fromIndex + k);
    sequence.erase(sequence.begin() + fromIndex, sequence.begin() + fromIndex + k);
    sequence.insert(sequence.begin() + toIndex, movedBlock.begin(), movedBlock.end());
    beginIndex = std::min(fromIndex, toIndex);
    endIndex = std::max(fromIndex, endIndex) + k;
}

void KInsert::commit() {

}

void KInsert::revert() {
    std::vector<unsigned int>& sequence = heuristic->getSequence();
    std::vector<unsigned int> movedBlock;
    movedBlock.insert(movedBlock.end(), sequence.begin() + toIndex, sequence.begin() + toIndex + k);
    sequence.erase(sequence.begin() + toIndex, sequence.begin() + toIndex + k);
    sequence.insert(sequence.begin() + fromIndex, movedBlock.begin(), movedBlock.end());
    //std::cout << "revert k " << k << " " << fromIndex << " " << toIndex << std::endl;
    //heuristic->displaySequence();  
}

// TODO ?
bool KInsert::doNothing() const {
    return false; 
}