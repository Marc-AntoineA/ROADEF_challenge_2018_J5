#pragma once

#include "glassLocation.h"
#include "redMonster.h"

#include <vector>

class GlassCutter {
    public:

    void initWithSequence(const std::vector<unsigned int>& sequence);
    unsigned int getCurrentScore() { return currentScore; }
    
    bool computeCutAndReturnIfFeasable() const;


    private:
    void build();
    
    unsigned int currentScore;
    std::vector<GlassLocation> locations;
    std::vector<RedMonster> monsters;
};