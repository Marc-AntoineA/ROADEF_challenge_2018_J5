#pragma once

#include "../GlassCutter/glassLocation.h"
#include "../GlassCutter/redMonster.h"
#include "../GlassCutter/glassNode.h"
#include "../GlassCutter/glassStack.h"
#include "../GlassData/glassInstance.h"

#include <vector>

class GlassCutter {
    public:

    GlassCutter(GlassInstance* instance, std::vector<unsigned int>& sequence);
    void setSequence(std::vector<unsigned int>& sequence) { this->sequence = sequence; }
    void cut();
    unsigned int getCurrentScore();
    
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
    unsigned int deepScore(const GlassLocation& location);
    void revert();
    std::vector<GlassLocation> getLocationsForItemIndexAndIncreaseBinIdIfNecessary(unsigned int itemIndex);
    RedMonster* currentMonster() { return &monsters[currentBinId]; }
    GlassNode* currentNode() { return &nodes[currentBinId]; }
    std::vector<GlassLocation>* currentLocations() { return &locations[currentBinId]; }

    GlassInstance* instance;
    
    unsigned int currentBinId;

    std::vector<std::vector<GlassLocation> > locations;
    std::vector<RedMonster> monsters;
    std::vector<GlassNode> nodes;
    std::vector<GlassStack> stacks;
    std::vector<unsigned int>& sequence;
    unsigned int nbRollbacks;
    unsigned int nbAttempts;
};