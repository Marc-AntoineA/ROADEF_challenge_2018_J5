#include "../GlassCutter/glassCutter.h"
#include "../GlassData/glassConstants.h"

#include <limits>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <sstream>

GlassCutter::GlassCutter(GlassInstance* instance, std::vector<unsigned int>& sequence)
    :instance(instance), sequence(sequence) {
    currentBinId = 0;
    nbRollbacks = 0;
    nbAttempts = 0;
    nbInfeasible = 0;
    currentSequenceIndex = 0;
    xLimit = NB_PLATES * WIDTH_PLATES;
    buildStacks();
    buildMonsters();
    buildNodes();
    buildLocations();
    resetErrorsStatistics();
}

void GlassCutter::reset() {
    currentBinId = 0;
    xLimit = NB_PLATES * WIDTH_PLATES;
    currentSequenceIndex = 0;
    for (GlassNode& node: nodes) 
        node.reset();
    for (RedMonster& monster: monsters)
        monster.reset();
    for (GlassStack& stack: stacks)
        stack.reset();
    for (std::vector<GlassLocation>& locationsTmp: locations) 
        locationsTmp.clear();
}

std::vector<GlassLocation> GlassCutter::getLocationsForItemIndexAndIncreaseBinIdIfNecessary(unsigned int itemIndex) {
    std::vector<GlassLocation> locations = currentMonster()->getLocationsForItemIndex(itemIndex);
    while (locations.empty()) {
        incrBinId();
        locations = currentMonster()->getLocationsForItemIndex(itemIndex);
    }
    return locations;
}

void GlassCutter::revertPlatesUntilSequenceIndex(unsigned int sequenceIndex) {
    currentBinId++;
    while (currentSequenceIndex >= sequenceIndex) {
        for (const GlassLocation& location: locations[currentBinId]){
            stacks[location.getStackId()].push();
            currentSequenceIndex--;
        }
        locations[currentBinId].clear();
        monsters[currentBinId].reset();
        nodes[currentBinId].reset();
        if (currentBinId > 0)
            currentBinId--;   
        else
            break;
    }
}

double GlassCutter::getCurrentBigNodeSurfaceOccupation() {
    if (currentNode()->getNbItems() == 0) return 0;
    std::vector<GlassNode>& sons = currentNode()->getSons();
    if (sons.back().getNbItems() == 0) {
        return sons[sons.size() - 2].getSurfaceOccupation();
    }
    return sons.back().getSurfaceOccupation();
}

void GlassCutter::cut(unsigned int depth){
    //displayStacks();
    unsigned int nbScore = 0;
    unsigned int nbNot = 0;
    if (VERBOSE) std::cout << "InitWithSequence en cours..." << std::endl;
    while (currentSequenceIndex < sequence.size()) {
        unsigned int stackId = sequence[currentSequenceIndex];
        if (VERBOSE) std::cout << "StackId#" << stackId;
        unsigned int itemIndex = stacks[stackId].top();
        if (VERBOSE) std::cout << " & item#" << itemIndex << std::endl;
        const std::vector<GlassLocation>& locations = getLocationsForItemIndexAndIncreaseBinIdIfNecessary(itemIndex);
        GlassLocation bestLocation;
        double bestScore = 0;
        for (const GlassLocation& location: locations) {
            if (currentBinId * WIDTH_PLATES + location.getXW() > xLimit) continue;
            if (!fullAttempt(location)) continue;
            double currentScore = fullDeepScore(currentSequenceIndex, depth);
            revert();
            if (currentScore >= bestScore) {
                bestScore = currentScore;
                bestLocation = location;
            }
        }
        //std::cout << bestScore << std::endl;
        if (bestScore > 0) {
            assert(currentBinId*WIDTH_PLATES + bestLocation.getXW() <= xLimit);
            //std::cout << "Location choisie (score " << bestScore << ") " << bestLocation << std::endl; 
            // fullAttempt is necessary due to the new way of counting the score
            // (tree is necessary)       
            if(!fullAttempt(bestLocation)) {throw std::runtime_error("???");}
            currentSequenceIndex++;
        } else {
            incrBinId();
            if (currentBinId * WIDTH_PLATES > xLimit) return;
        }
    }
    unsigned int xMax = currentBinId * WIDTH_PLATES + getXMax();
    xLimit = std::min(xMax, xLimit);
}

bool GlassCutter::isLessGood() {
    return currentBinId * WIDTH_PLATES + getLazyXMax() > xLimit; 
}

double GlassCutter::quickEvaluateLocation(double lazy) {
    double xMax = lazy ? getLazyXMax() : getXMax();
    //double monsterArea = currentMonster()->computeArea();

    return 1 - xMax/WIDTH_PLATES;
}

double GlassCutter::buildLazyDeepScoreTree(unsigned int sequenceIndex, unsigned int depth, ScoredLocationTree& tree) {
    tree.sons.clear();

    tree.scoredLocation.location = currentLocations()->back();

    if (isLessGood()) {
        tree.scoredLocation.score = -1000000;
        return tree.scoredLocation.score;
    }

    double currentScore = 0;

    if (depth == 0 || sequenceIndex + 1 == sequence.size()) {
        tree.scoredLocation.score = quickEvaluateLocation(LAZY);
        return tree.scoredLocation.score;
    }

    unsigned int itemIndex = stacks[sequence[sequenceIndex + 1]].top();
    const std::vector<GlassLocation>& currentLocations = currentMonster()->getLocationsForItemIndex(itemIndex);
    if (currentLocations.size() == 0) return 0;

    for (const GlassLocation& location: currentLocations) {
        if (!lazyAttempt(location));
        tree.sons.push_back(ScoredLocationTree());
        ScoredLocationTree &son = tree.sons.back();
        currentScore = std::max(currentScore, 1. + buildLazyDeepScoreTree(sequenceIndex + 1, depth - 1, son));
        revert();
    }
    
    tree.scoredLocation.score = currentScore;
    return tree.scoredLocation.score;
}

void GlassCutter::ScoredLocationTree::sort() {
    std::sort(sons.begin(), sons.end());
}

void GlassCutter::ScoredLocationTree::display() const {
    display(" ");
}

void GlassCutter::ScoredLocationTree::display(std::string prefix) const {
    std::cout << prefix << scoredLocation.location << " | " << scoredLocation.score << std::endl;
    for (const ScoredLocationTree& son: sons) {
        son.display(prefix + " ");
    }
}

double GlassCutter::treeScore(ScoredLocationTree& tree) {
    tree.sort();
    double bestScore = quickEvaluateLocation(!LAZY);
    
    if (tree.sons.empty()) {
        return bestScore;
    }
    
    tree.sort();
    for (ScoredLocationTree& son: tree.sons) {
        //std::cout << bestScore << " >= " << son.scoredLocation.score << std::endl;
        if (bestScore >= son.scoredLocation.score) break;
        const GlassLocation& location = son.scoredLocation.location;
        if (!fullAttempt(location)) continue;
        bestScore = std::max(bestScore, 1 + treeScore(son));
        revert();
    } 
    return bestScore;
}

double GlassCutter::fullDeepScore(unsigned int sequenceIndex, unsigned int depth) {
    //std::cout << currentLocations()->back() << std::endl;
    scoredTree.sons.clear();
    buildLazyDeepScoreTree(sequenceIndex, depth, scoredTree);
    //tree.display();
    //std::cout << "\t" << treeScore(scoredTree) << std::endl;
    return treeScore(scoredTree);
}

unsigned int GlassCutter::getCurrentScore() {
    return (currentBinId*WIDTH_PLATES + getXMax())*HEIGHT_PLATES - instance->getItemsArea();
}

void GlassCutter::buildStacks() {
    stacks.clear();
    for (const GlassItem& item: instance->getItems()) {
        unsigned int stackId = item.getStackId();
        if (stacks.size() <= stackId) {
            for (unsigned int k = stacks.size(); k < stackId + 1; k++)
                stacks.push_back(GlassStack(instance, k));
        }
        stacks[stackId].insert(item);
    }
    if (VERBOSE) displayStacks();
}

void GlassCutter::displayStacks() const {
    for (const GlassStack& stack: stacks) 
        std::cout << stack;
}

void GlassCutter::displayLocations() const {
    std::cout << "==================" << std::endl;
    for (const std::vector<GlassLocation> locationsIt: locations) {
        for (const GlassLocation& location: locationsIt) 
            std::cout << location << std::endl;
    }
    std::cout << "==================" << std::endl;
}

void GlassCutter::buildNodes() {
    nodes.clear();
    nodes.resize(NB_PLATES);
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
        nodes[plateIndex].setPlateIndex(plateIndex);
        nodes[plateIndex].setInstance(instance);
    }
}

void GlassCutter::buildMonsters() {
    monsters.clear();
    monsters.resize(NB_PLATES);
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
        monsters[plateIndex].setPlateIndex(plateIndex);
        monsters[plateIndex].setInstance(instance);
    }
}

void GlassCutter::buildLocations() {
    locations.clear();
    locations.resize(NB_PLATES);
}

void GlassCutter::incrBinId() {
    currentBinId++;
    if(VERBOSE) std::cout << "Incr bin id to " << currentBinId << std::endl;
}

void GlassCutter::decrBinId() {
    if (currentBinId == 0) return;
    currentMonster()->reset();
    currentLocations()->clear();
    currentBinId--;
    //std::cout << "decrBinId" << std::endl;
}

void GlassCutter::setBinId(unsigned int binId) {
    currentBinId = binId;
}

bool GlassCutter::checkTreeFeasibilityAndBuildCurrentNode() {
    try {
        currentNode()->buildNodeAndReturnNbItemsCuts(currentLocations()->begin(), currentLocations()->end());
        return true;
    } catch (const std::runtime_error& e) {
        //std::cout << e.what() << std::endl;
        addErrorStatistic(e.what());
        /*std::cout << "---|||---|||---" << std::endl;
        std::cout << e.what() << std::endl;
        displayLocations();*/
        nbInfeasible++;
        return false;
    }
}

bool GlassCutter::lazyAttempt(const GlassLocation& location) {
    return attempt(location, true); 
}

bool GlassCutter::fullAttempt(const GlassLocation& location) {
    return attempt(location, false);
}

// TODO build pour guillotine cut?
bool GlassCutter::attempt(const GlassLocation& location, bool fast) {
    //if (VERBOSE) std::cout << "attempt location: " << location << std::endl;
    setBinId(location.getBinId());
    nbAttempts++;
    stacks[location.getStackId()].pop();
    currentMonster()->incrRedMonster(location);
    currentLocations()->push_back(location);
    if (fast) return true;
    if (!checkTreeFeasibilityAndBuildCurrentNode()) {
        revert();
        //std::cout << (*currentMonster()) << std::endl;
        return false;
    }
    return true;
}

void GlassCutter::revert() {
    nbRollbacks++;
    while (currentLocations()->empty()) {
        if (currentBinId == 0) throw std::runtime_error("Rollback impossible (not enough items).");
        decrBinId();
    }
    unsigned int stackId = currentLocations()->back().getStackId();
    if (VERBOSE) std::cout << "revert for location " << currentLocations()->back() << std::endl;
    stacks[stackId].push();
    currentLocations()->pop_back();
    currentMonster()->revert();
    if (currentLocations()->empty()) 
        decrBinId();    
}

void GlassCutter::saveBest(std::string name) {
    if (VERBOSE) std::cout << "Sauvegarde de l'instance " << name << std::endl;
    std::ofstream outputFile;
    outputFile.open(name.c_str());
    outputFile << "PLATE ID;NODE ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT" << std::endl;

    unsigned int lastBinId = 0;
    for (unsigned int binId=0; binId < locations.size(); binId++){
        if (locations[binId].empty()) break;
        lastBinId++;
    }
    int maxSonId = -1;
    for (unsigned int binId=0; binId < lastBinId; binId++){
        nodes[binId].reset();
        nodes[binId].buildNodeAndReturnNbItemsCuts(locations[binId].begin(), locations[binId].end());
        maxSonId = nodes[binId].saveNode(outputFile, maxSonId + 1, -1, binId == lastBinId - 1);
    }

    std::cout << "Sauvegarde effectuée avec succès " << std::endl;
}

void GlassCutter::addErrorStatistic(std::string errorMessage) {
    if (errorMessage == "Trimming failed (more than 1 item)" 
        || errorMessage == "Trimming failed (more than 1 cut)"){
        nbTrimmingFailed++;
        return;
    }

    if (errorMessage == "Tree too depth") {
        nbTreeTooDepth++;
        return;
    }
    
    if (errorMessage == "Waste too small") {
        nbWasteTooSmall++;
        /*std::cout << "======" << std::endl;
        displayLocations();*/
        return;
    }

    if (errorMessage == "Trimming failed (prechecked)") {
        nbTrimmingPreChecked++;
        return;
    }

    //8std::cout << errorMessage << std::endl;
}

void GlassCutter::resetErrorsStatistics() {
    nbTrimmingFailed = 0;
    nbTreeTooDepth = 0;
    nbWasteTooSmall = 0;
    nbTrimmingPreChecked = 0;
}

void GlassCutter::displayErrorStatistics() const {
    std::cout << "Waste too small: " << nbWasteTooSmall << std::endl;
    std::cout << "Tree too depth: " << nbTreeTooDepth << std::endl;
    std::cout << "NbTrimmingFailed: " << nbTrimmingFailed << std::endl;
    std::cout << "NbTrimmingPreChecked: " << nbTrimmingPreChecked << std::endl;
}

unsigned int GlassCutter::computeMaxScorePossible() {
    int availableArea = WIDTH_PLATES*HEIGHT_PLATES - currentMonster()->computeArea();
    unsigned int maxIndex = currentSequenceIndex + 1;
    while (maxIndex < sequence.size() && availableArea > 0) {
        availableArea -= instance->getItem(sequence[maxIndex]).getArea();
        maxIndex++;
    }
    return maxIndex - currentSequenceIndex;
}

void GlassCutter::displayStatistics() const {
    std::cout << "Attempts: " << nbAttempts;
    std::cout << "\tNb infeasible: " << nbInfeasible;
    std::cout << "\tNb Rollbacks: " << nbRollbacks;
    std::cout << std::endl;
}

double GlassCutter::getSurfacePlateOccupation(unsigned int plateIndex) const {
    return nodes[plateIndex].getSurfaceOccupation();
}

unsigned int GlassCutter::getXMax() {
    assert(currentMonster()->getXMax() <= currentNode()->getXMax());
    return currentNode()->getXMax();
}

unsigned int GlassCutter::getLazyXMax() {
    return currentMonster()->getXMax();
}