#pragma once

#include "../GlassKernel/glassMove.h"
#include "../GlassData/glassInstance.h"
#include "../GlassCutter/glassCutter.h"

#include <vector>

class Heuristic {

    public:
    Heuristic(GlassInstance instance);

    void displaySequence();

    private:
    void initRandomlySequence();

    GlassInstance instance;
    GlassCutter cutter;

    std::vector<GlassMove> poolMoves;

    std::vector<unsigned int> sequence;
    unsigned int score;
    std::vector<unsigned int> bestSequence;
    unsigned int bestScore;
};