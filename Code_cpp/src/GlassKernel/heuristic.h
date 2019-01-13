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
    void localSearch(unsigned int depth);
    void displayMoveStatistics();
    unsigned int getBestScore() const { return bestScore; }
    void saveBest(std::string name);

    private:
    void initRandomlySequence();
    void buildMoves(); 
    void resetStacks();
    unsigned int computeScore(unsigned int depth);
    unsigned int computeScore(unsigned int depth, unsigned int beginSequenceIndex);
    GlassInstance instance;
    GlassCutter cutter;

    std::vector<GlassMove*> poolMoves;

    std::vector<unsigned int> sequence;
    unsigned int bestScore;
};