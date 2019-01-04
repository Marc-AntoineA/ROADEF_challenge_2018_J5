#pragma once

#include "../GlassCutter/glassLocation.h"
#include "../GlassCutter/redMonster.h"
#include "../GlassCutter/glassNode.h"
#include "../GlassData/glassInstance.h"

#include <vector>

class GlassCutter {
    public:

    GlassCutter(GlassInstance* instance);
    void initWithSequence(const std::vector<unsigned int>& sequence);
    unsigned int getCurrentScore() { return currentScore; }
    
    bool computeCutAndReturnIfFeasable() const;

    private:
    void build();
    void reset();
    
    GlassInstance* instance;

    unsigned int currentScore;
    unsigned int currentBinId;

    std::vector<GlassLocation> locations;
    std::vector<RedMonster> monsters;
    std::vector<GlassNode> nodes;
};