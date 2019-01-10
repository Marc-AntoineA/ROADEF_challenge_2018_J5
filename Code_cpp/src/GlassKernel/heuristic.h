#pragma once

#include "../GlassData/glassInstance.h"
#include "../GlassCutter/glassCutter.h"
#include "../GlassKernel/glassMove.h"

#include <vector>

class Heuristic {

    public:
    Heuristic(GlassInstance instance);

    std::vector<unsigned int>& getSequence() { return sequence; }
    void displaySequence();
    unsigned int glassRandint(unsigned int first, unsigned int last);
    unsigned int computeScore();
    void localSearch();

    private:
    void initRandomlySequence();
    void buildMoves(); 
    void resetStacks();
    GlassInstance instance;
    GlassCutter cutter;

    std::vector<GlassMove*> poolMoves;

    std::vector<unsigned int> sequence;
    unsigned int score;
    std::vector<unsigned int> bestSequence;
    unsigned int bestScore;
};