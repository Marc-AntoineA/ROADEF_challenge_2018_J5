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
    scoredTree = ScoredLocationTree();
    unsigned int nbScore = 0;
    unsigned int nbNot = 0;
    if (VERBOSE) std::cout << "InitWithSequence en cours..." << std::endl;
    while (currentSequenceIndex < sequence.size()) {
        unsigned int stackId = sequence[currentSequenceIndex];
        unsigned int itemIndex = stacks[stackId].top();
        std::cout << "Placement de l'item# " << itemIndex << std::endl;
        //std::cout << "currentbin id " << currentBinId << std::endl;
        buildLazyDeepScoreTree(currentSequenceIndex, depth, scoredTree);
        //std::cout << "Current bin id (after build) " << currentBinId << std::endl;
        scoredTree.display();
        if(computeBestLocationAndApplyIfNecessary()) {
            currentSequenceIndex++;
            //std::cout << currentSequenceIndex << std::endl;
        } else {
            incrBinId();
            //std::cout << " binId: " << currentBinId << std::endl;
            if (currentBinId * WIDTH_PLATES > xLimit) return;
        }
    }
    unsigned int xMax = currentBinId * WIDTH_PLATES + getXMax();
    xLimit = std::min(xMax, xLimit);
}

double GlassCutter::quickEvaluateLocation(double lazy) {
    double xMax = lazy ? getLazyXMax() : getXMax();
    //double monsterArea = currentMonster()->computeArea();

    return 1 - xMax/WIDTH_PLATES;
}

double GlassCutter::buildLazyDeepScoreTree(unsigned int sequenceIndex, unsigned int depth, ScoredLocationTree& tree) {
    tree.sons.clear();

    if (isLessGood()) {
        tree.scoredLocation.score = -1000000;
        return tree.scoredLocation.score;
    }

    double currentScore = 0;

    if (depth == 0 || sequenceIndex == sequence.size()) {
        tree.scoredLocation.score = quickEvaluateLocation(LAZY);
        return tree.scoredLocation.score;
    }
    unsigned int itemIndex = stacks[sequence[sequenceIndex]].top();
    const std::vector<GlassLocation>& currentLocations = currentMonster()->getLocationsForItemIndex(itemIndex);
    if (currentLocations.size() == 0) return 0;

    for (const GlassLocation& location: currentLocations) {
        if (!lazyAttempt(location));
        tree.sons.push_back(ScoredLocationTree(location));
        ScoredLocationTree &son = tree.sons.back();
        currentScore = std::max(currentScore, 1. + buildLazyDeepScoreTree(sequenceIndex + 1, depth - 1, son));
        revertSameBin();
    }
    
    tree.scoredLocation.score = currentScore;
    return tree.scoredLocation.score;
}

GlassCutter::ScoredLocation GlassCutter::treeScore(ScoredLocationTree& tree) {
    tree.sort();
    double bestScore = quickEvaluateLocation(LAZY) - 1; // TODO potentiel bug ici avec les attempt
    
    unsigned int bestLocationIndex = tree.sons.size();
    tree.sort();
    for (unsigned int index = 0; index < tree.sons.size(); index++) {
        ScoredLocationTree& son = tree.sons[index];
        ScoredLocation& scoredLocation = son.scoredLocation;
        if (bestScore >= scoredLocation.score) break;
        const GlassLocation& location = scoredLocation.location;
        switch (scoredLocation.feasible) {
        case NOT_FEASIBLE:
            continue;

        case NOT_TESTED:
            if (!fullAttempt(location)) {
                scoredLocation.feasible = NOT_FEASIBLE;
                continue;
            }
            break;

        case FEASIBLE:
            if (!lazyAttempt(location)) {
                scoredLocation.feasible = NOT_FEASIBLE;
                continue;
            }
            break;
        }
        scoredLocation.feasible = FEASIBLE;
        double currentScore = 1. + treeScore(son).score;
        //std::cout << scoredLocation.location << " : " << currentScore << std::endl;
        if (currentScore >= bestScore) {
            bestLocationIndex = index;
            bestScore = currentScore;
        }
        revertSameBin();
    } 
    //std::cout << bestScore << std::endl;
    //std::cout << bestLocationIndex << std::endl;
    if (bestLocationIndex < tree.sons.size() && bestScore > 0) {
        //std::cout << bestScore << " vs " << bestLocationIndex << " < " << tree.sons.size() << std::endl;
        return ScoredLocation(tree.sons[bestLocationIndex].scoredLocation.location, bestScore);
    }

    return ScoredLocation(GlassLocation(), bestScore);
}

bool GlassCutter::computeBestLocationAndApplyIfNecessary() {
    //std::cout << "computing best location " << std::endl;
    //scoredTree.display();
    ScoredLocation best = treeScore(scoredTree);

    if (best.location.getInstance() == NULL) return false;
    if (best.score <= 0) return false;
    
    std::cout << best.score << std::endl;
    lazyAttempt(best.location);
    for (unsigned int sonIndex = 0; sonIndex < scoredTree.sons.size(); sonIndex++) {
        const GlassLocation& currentLocation = scoredTree.sons[sonIndex].scoredLocation.location;
        if (currentLocation == best.location) {
            std::cout << "? " << std::endl;
            scoredTree = scoredTree.sons[sonIndex];
            std::cout << "line 201" << std::endl;
            std::cout << "Applied " << scoredTree.scoredLocation.location << std::endl;
            return true;
        }
    }
    assert(false);
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
    scoredTree = ScoredLocationTree();
    if(VERBOSE) std::cout << "Incr bin id to " << currentBinId << std::endl;
}

void GlassCutter::decrBinId() {
    if (currentBinId == 0) return;
    currentMonster()->reset();
    currentLocations()->clear();
    currentBinId--;
    //std::cout << "decrBinId" << std::endl;
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

void GlassCutter::revertSameBin() {
    unsigned int stackId = currentLocations()->back().getStackId();
    if (VERBOSE) std::cout << "revert for location " << currentLocations()->back() << std::endl;
    stacks[stackId].push();
    currentLocations()->pop_back();
    currentMonster()->revert();
}

void GlassCutter::revert() {
    nbRollbacks++;
    while (currentLocations()->empty()) {
        if (currentBinId == 0) throw std::runtime_error("Rollback impossible (not enough items).");
        decrBinId();
    }
    revertSameBin();
    if (currentLocations()->empty()) {
        decrBinId();    
    }
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

void GlassCutter::displayErrorStatistics() const {
    std::cout << "Waste too small: " << nbWasteTooSmall << std::endl;
    std::cout << "Tree too depth: " << nbTreeTooDepth << std::endl;
    std::cout << "NbTrimmingFailed: " << nbTrimmingFailed << std::endl;
    std::cout << "NbTrimmingPreChecked: " << nbTrimmingPreChecked << std::endl;
}

bool GlassCutter::isLessGood() {
    return currentBinId * WIDTH_PLATES + getLazyXMax() > xLimit; 
}

void GlassCutter::ScoredLocationTree::sort() {
    std::sort(sons.begin(), sons.end());
}

void GlassCutter::ScoredLocationTree::display() const {
    display(" ");
}

void GlassCutter::ScoredLocationTree::display(std::string prefix) const {
    std::cout << prefix;
    if (scoredLocation.location.getInstance() == NULL)
        std::cout << "(null)";
    else 
        std::cout << scoredLocation.location;

    std::cout << " | " << scoredLocation.score << " " << scoredLocation.feasible<< std::endl;

    for (const ScoredLocationTree& son: sons) {
        son.display(prefix + " ");
    }
}

void GlassCutter::setBinId(unsigned int binId) {
    currentBinId = binId;
}