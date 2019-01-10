#pragma once

#include "../../GlassKernel/glassMove.h"

class Swap: public GlassMove {
    public:
    Swap(Heuristic* heuristic) : GlassMove(heuristic) { this->name="Swap";}
    virtual bool attempt();
    void commit();
    void revert();

    private:
    unsigned int firstIndex;
    unsigned int lastIndex;
};