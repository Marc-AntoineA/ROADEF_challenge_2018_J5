#pragma once

class Heuristic;

class GlassMove {
    public:
    GlassMove(Heuristic* heuristic): heuristic(heuristic) {}
    GlassMove() {}

    virtual void commit() {}
    virtual void revert() {}
    virtual bool attempt() {return true;}

    protected:
    Heuristic* heuristic;
};