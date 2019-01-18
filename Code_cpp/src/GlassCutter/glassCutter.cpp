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
    std::cout << "cut " << std::endl;
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
        if (bestScore > 0) {
            assert(currentBinId*WIDTH_PLATES + bestLocation.getXW() <= xLimit);
            //std::cout << "Location choisie (score " << bestScore << ") " << bestLocation << std::endl;        
            if(!lazyAttempt(bestLocation)) {throw std::runtime_error("???");} // full attemp is useless
            currentSequenceIndex++;
        } else {
            incrBinId();
            if (currentBinId * WIDTH_PLATES > xLimit) return;
        }
    }
    unsigned int xMax = currentBinId * WIDTH_PLATES + currentMonster()->getXMax();
    xLimit = std::min(xMax, xLimit);
    //displayLocations();
    /*std::cout << "SCORE : " << getCurrentScore() << std::endl;
    std::cout << "nbRollbacks : " << nbRollbacks << std::endl;
    std::cout << "nbAttempts : " << nbAttempts << std::endl;
    std::cout << "nbInfeasible : " << nbInfeasible << std::endl;*/
}

bool GlassCutter::isLessGood() {
    return currentBinId * WIDTH_PLATES + currentMonster()->getXMax() > xLimit; 
}

double GlassCutter::lazyDeepScore(unsigned int sequenceIndex, unsigned int depth) {
    if (isLessGood()) return -1000000000;

    double currentScore = 1 - currentMonster()->getXMax()/(double)WIDTH_PLATES;

    if (depth == 0 || sequenceIndex + 1 == sequence.size()) {
        return currentScore;
    }

    unsigned int itemIndex = stacks[sequence[sequenceIndex + 1]].top();
    const std::vector<GlassLocation>& currentLocations = currentMonster()->getLocationsForItemIndex(itemIndex);

    for (const GlassLocation& location: currentLocations) {
        if (!lazyAttempt(location));
        currentScore = std::max(currentScore, 1. + lazyDeepScore(sequenceIndex + 1, depth - 1));
        revert();
    }
    return currentScore;
}

double GlassCutter::fullDeepScore(unsigned int sequenceIndex, unsigned int depth) {
    if (isLessGood()) return -10000000000;

    double currentScore = getCurrentBigNodeSurfaceOccupation();

    if (depth == 0 || sequenceIndex + 1 == sequence.size()) {
        return currentScore;
    }
   
    unsigned int itemIndex = stacks[sequence[sequenceIndex + 1]].top();
    const std::vector<GlassLocation>& currentLocations = currentMonster()->getLocationsForItemIndex(itemIndex);
 
    // First: evaluate lazy deep score for each location
    std::vector<ScoredLocation> scoredLocations;
    for (unsigned int locationIndex = 0; locationIndex < currentLocations.size(); locationIndex++) {
        const GlassLocation& locationBis = currentLocations[locationIndex];
        if (!lazyAttempt(locationBis)) { continue; }
        double lazyScore = 1 + lazyDeepScore(sequenceIndex + 1, depth - 1);
        revert();
        //currentScore = std::max(currentScore, 1. + lazyScore);
        if (lazyScore < 0) continue;
        scoredLocations.push_back(ScoredLocation(lazyScore, locationIndex));
        //std::cout << lazyScore << std::endl;
    }

    std::sort(scoredLocations.begin(), scoredLocations.end());

    std::vector<double> tmpScores;
    // Second: We know the best scores possible for each location
    // If a scored location is possible, we have our best score
    for (unsigned int locationIndex = 0; locationIndex < scoredLocations.size(); locationIndex++) {
        const ScoredLocation& scoredLocation = scoredLocations[locationIndex];
        //if (currentScore >= scoredLocation.score) break;
        if (!fullAttempt(currentLocations[scoredLocation.locationIndex])) { continue; }
        double deepScore = fullDeepScore(sequenceIndex + 1, depth - 1);
        revert();
        tmpScores.push_back(1. + deepScore);
        currentScore = std::max(currentScore, 1. + deepScore);
        //std::cout << currentScore << " vs " << scoredLocation.score << std::endl;
    }    

    if (tmpScores.size() < 4) return currentScore;
    std::cout << "===============" << std::endl;
    for (double score: tmpScores) {
        std::cout << score << " ";
    }
    std::cout << " >>>> " << currentScore << std::endl;

    for (const ScoredLocation& location: scoredLocations) {
        std::cout << location.score << " ";
    }
    std::cout << " >>>> " << currentScore << std::endl;
    return currentScore;
}

unsigned int GlassCutter::getCurrentScore() {
    /*std::cout << currentBinId << std::endl;
    std::cout << currentMonster()->getXMax() << std::endl;
    std::cout << instance->getItemsArea() << std::endl;
    std::cout << (currentBinId*WIDTH_PLATES + currentMonster()->getXMax())*HEIGHT_PLATES - instance->getItemsArea() << std::endl;*/
    return (currentBinId*WIDTH_PLATES + currentMonster()->getXMax())*HEIGHT_PLATES - instance->getItemsArea();
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