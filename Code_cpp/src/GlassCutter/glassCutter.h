#pragma once

#include "../GlassCutter/glassLocation.h"
#include "../GlassCutter/redMonster.h"
#include "../GlassCutter/glassNode.h"
#include "../GlassCutter/glassStack.h"
#include "../GlassData/glassInstance.h"

#include <vector>

#define LAZY true


class GlassCutter {

    public:
    GlassCutter(GlassInstance* instance, std::vector<unsigned int>& sequence);
    void setSequence(std::vector<unsigned int>& sequence) { this->sequence = sequence; }
    bool cut(unsigned int depth);
    unsigned int getCurrentScore();
    
    void displayErrorStatistics() const;
    void displayStacks() const;
    void displayLocations() const;
    void reset();
    void revertPlatesUntilSequenceIndex(unsigned int sequenceIndex);
    void saveBest(std::string name);
    void displayStatistics() const;
    double getSurfacePlateOccupation(unsigned int plateIndex) const;

    private:
    enum Feasible {
        FEASIBLE,
        NOT_FEASIBLE,
        NOT_TESTED
    };    
    
    struct ScoredLocation {
        double score;
        GlassLocation location;
        Feasible feasible;
        ScoredLocation() : feasible(NOT_TESTED) {}
        ScoredLocation(GlassLocation location, double score) : location(location), score(score), feasible(NOT_TESTED) {}
        bool operator<(const ScoredLocation& otherLocation) const {
            return score > otherLocation.score;
        }
    };

    struct ScoredLocationTree {

        ScoredLocation scoredLocation;
        std::vector<ScoredLocationTree*> sons;

        ScoredLocationTree() {}
        ScoredLocationTree(GlassLocation location) : scoredLocation(location, -1) {}
        void sort();
        void reset();
        void display() const;
        void display(std::string prefix) const;

        static bool compareTrees(ScoredLocationTree* a, ScoredLocationTree* b) {
            return a->scoredLocation < b->scoredLocation;
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
    double buildLazyDeepScoreTree(unsigned int sequenceIndex, unsigned int depth, ScoredLocationTree* tree);
    void buildLazyDeepScoreTreeAndIncreaseBinIdIfNecessary(unsigned int itemIndex);
    double lazyDeepScore(unsigned int sequenceIndex, unsigned int depth, ScoredLocationTree* tree); 
    bool computeBestLocationAndApplyIfNecessary();
    double deepScore(unsigned int sequenceIndex, unsigned int depth, bool fast);
    ScoredLocation treeScore(ScoredLocationTree* tree);
    double evaluateLocation(unsigned int sequenceIndex, unsigned int depth);
    double quickEvaluateLocation(double lazy);
    bool isLessGood();     
    
    double getCurrentBigNodeSurfaceOccupation();
    unsigned int computeMaxScorePossible();
    void revert();
    void revertSameBin();
    bool checkTreeFeasibilityAndBuildCurrentNode();
    unsigned int getXMax();
    unsigned int getLazyXMax();
    std::vector<GlassLocation> getLocationsForItemIndexAndIncreaseBinIdIfNecessary(unsigned int itemIndex);
    RedMonster* currentMonster() { return &monsters[currentBinId]; }
    GlassNode* currentNode() { return &nodes[currentBinId]; }
    std::vector<GlassLocation>* currentLocations() { return &locations[currentBinId]; }

    GlassInstance* instance;
    
    unsigned int currentBinId;
    unsigned int currentSequenceIndex;

    ScoredLocationTree* scoredTree;
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