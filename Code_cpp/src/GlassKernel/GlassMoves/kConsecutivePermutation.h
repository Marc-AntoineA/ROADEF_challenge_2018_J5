#pragma once

#include "../../GlassKernel/glassMove.h"

#include <vector>

class KConsecutivePermutation : public GlassMove {
    public:
    KConsecutivePermutation(Heuristic* heuristic, unsigned int k);
    void attempt(unsigned int& beginIndex, unsigned int& endIndex);
    void commit();
    void revert();

    private:
    void applyPermutation();
    void revertPermutation();
    void initPermutation();
    void mixPermutation();
    bool doNothing();
    void displayPermutation();  
    unsigned int k;
    std::vector<unsigned int> permutation;
    unsigned int firstIndex;
};