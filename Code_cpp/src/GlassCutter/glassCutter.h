#pragma once

#include "../GlassCutter/glassLocation.h"
#include "../GlassCutter/redMonster.h"
#include "../GlassCutter/glassNode.h"
#include "../GlassCutter/glassStack.h"
#include "../GlassData/glassInstance.h"

#include <vector>

class GlassCutter {
    public:

    GlassCutter(GlassInstance* instance);
    void initWithSequence(const std::vector<unsigned int>& sequence);
    unsigned int getCurrentScore() { return currentScore; }
    
    bool computeCutAndReturnIfFeasable() const;
    void displayStacks();

    private:
    void incrBinId();
    void decrBinId();
    void buildStacks();
    void buildMonsters();
    void buildNodes();
    void buildLocations();
    void build();
    void reset();
    bool attempt(const GlassLocation& location);
    void revert();
    RedMonster* currentMonster() { return &monsters[currentBinId]; }
    GlassNode* currentNode() { return &nodes[currentBinId]; }
    std::vector<GlassLocation>* currentLocations() { return &locations[currentBinId]; }

    GlassInstance* instance;

    unsigned int currentScore;
    unsigned int currentBinId;

    std::vector<std::vector<GlassLocation> > locations;
    std::vector<RedMonster> monsters;
    std::vector<GlassNode> nodes;
    std::vector<GlassStack> stacks;

    unsigned int nbRollbacks;
    unsigned int nbAttempts;
};