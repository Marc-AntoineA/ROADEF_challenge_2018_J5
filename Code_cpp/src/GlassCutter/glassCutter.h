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
    
    void displayErrorStatistics() const;
    void displayStacks();
    void displayLocations();
    void reset();
    void revertPlatesUntilSequenceIndex(unsigned int sequenceIndex);
    void saveBest(std::string name);

    private:
    struct ScoredLocation {
        double score;
        unsigned int locationIndex;

        ScoredLocation(double score, unsigned int locationIndex)
            :score(score), locationIndex(locationIndex) {}

        bool operator<(const ScoredLocation& otherLocation) const {
            return score < otherLocation.score;
        }
    };

    void addErrorStatistic(std::string errorMessage); 
    void resetErrorsStatistics();
    void incrBinId();
    void decrBinId();
    void setBinId(unsigned int binId);
    void buildStacks();
    void buildMonsters();
    void buildNodes();
    void buildLocations();
    bool lazyAttempt(const GlassLocation& location);
    bool attempt(const GlassLocation& location, bool fast);
    bool fullAttempt(const GlassLocation& location);
    double lazyDeepScore(unsigned int sequenceIndex, unsigned int depth); 
    double fullDeepScore(unsigned int sequenceIndex, unsigned int depth);
    double deepScore(unsigned int sequenceIndex, unsigned int depth, bool fast);
    bool isLessGood();  
    unsigned int computeMaxScorePossible();
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

    unsigned int nbTrimmingFailed;
    unsigned int nbWasteTooSmall;
    unsigned int nbTreeTooDepth;
    unsigned int nbTrimmingPreChecked;

    unsigned int xLimit;
};