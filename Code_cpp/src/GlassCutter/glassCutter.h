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
    void cut(unsigned int depth);
    unsigned int getCurrentScore();
    
    void displayStacks();
    void displayLocations();
    void reset();
    void revertPlatesUntilSequenceIndex(unsigned int sequenceIndex);
    void saveBest(std::string name);

    private:
    void incrBinId();
    void decrBinId();
    void setBinId(unsigned int binId);
    void buildStacks();
    void buildMonsters();
    void buildNodes();
    void buildLocations();
    bool attempt(const GlassLocation& location);
    double deepScore(unsigned int sequenceIndex, unsigned int depth);
    void revert();
    bool checkTreeFeasibilityAndBuildCurrentNode();
    std::vector<GlassLocation> getLocationsForItemIndexAndIncreaseBinIdIfNecessary(unsigned int itemIndex);
    RedMonster* currentMonster() { return &monsters[currentBinId]; }
    GlassNode* currentNode() { return &nodes[currentBinId]; }
    std::vector<GlassLocation>* currentLocations() { return &locations[currentBinId]; }

    GlassInstance* instance;
    
    unsigned int currentBinId;
    unsigned int currentSequenceIndex;

    std::vector<std::vector<GlassLocation> > locations;
    std::vector<RedMonster> monsters;
    std::vector<GlassNode> nodes;
    std::vector<GlassStack> stacks;
    std::vector<unsigned int>& sequence;

    unsigned int nbRollbacks;
    unsigned int nbAttempts;
    unsigned int nbInfeasible;

    unsigned int xLimit;
};