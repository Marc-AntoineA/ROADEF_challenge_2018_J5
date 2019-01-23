#pragma once

#include "../../GlassKernel/glassMove.h"

class Swap: public GlassMove {
    public:
    Swap(Heuristic* heuristic) : GlassMove(heuristic) { this->name="Swap";}
    void attempt(unsigned int& beginIndex, unsigned int& endIndex);
    void commit();
    void revert();

    private:
    unsigned int firstIndex;
    unsigned int lastIndex;
};