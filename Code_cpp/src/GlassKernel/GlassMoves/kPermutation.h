#pragma once

#include "../../GlassKernel/glassMove.h"

#include <vector>

class KPermutation : public GlassMove {
    public:
    KPermutation(Heuristic* heuristic, unsigned int k);
    int attempt();
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
};