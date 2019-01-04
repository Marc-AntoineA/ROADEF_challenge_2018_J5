#pragma once

//#include "heuristic.h"

class Heuristic;

class GlassMove {
    public:
    GlassMove(Heuristic* heuristic): heuristic(heuristic){};
    GlassMove() {}

    virtual void commit() = 0;
    virtual void revert() = 0;
    virtual void attempt() = 0;

    private:
    Heuristic* heuristic;
};