#include "../GlassCutter/glassCutter.h"
#include "../GlassData/glassConstants.h"

#include <limits>
#include <cassert>
#include <fstream>
#include <sstream>

GlassCutter::GlassCutter(GlassInstance* instance, std::vector<unsigned int>& sequence)
    :instance(instance), sequence(sequence) {
    currentBinId = 0;
    nbRollbacks = 0;
    nbAttempts = 0;
    nbInfeasible = 0;
    currentSequenceIndex = 0;
    buildStacks();
    buildMonsters();
    buildNodes();
    buildLocations();
}

void GlassCutter::reset() {
    currentBinId = 0;
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

void GlassCutter::cut(unsigned int depth){
    //displayStacks();
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
            if (!attempt(location)) continue;
            double currentScore = deepScore(currentSequenceIndex, depth);
            revert();
            if (currentScore >= bestScore) {
                bestScore = currentScore;
                bestLocation = location;
            }
        }
        if (bestScore > 0) {
            //std::cout << "Location choisie (score " << bestScore << ") " << bestLocation << std::endl;        
            attempt(bestLocation);
            currentSequenceIndex++;
        } else {
            incrBinId();
        }
    }
    std::cout << "binId" << currentBinId << std::endl;
    //displayLocations();
    std::cout << "SCORE : " << getCurrentScore() << std::endl;
    std::cout << "nbRollbacks : " << nbRollbacks << std::endl;
    std::cout << "nbAttempts : " << nbAttempts << std::endl;
    std::cout << "nbInfeasible : " << nbInfeasible << std::endl;
}

double GlassCutter::deepScore(unsigned int sequenceIndex, unsigned int depth) {
    if (depth == 0 || sequenceIndex + 1 == sequence.size()) 
        return  1 - currentMonster()->getXMax()/(double)WIDTH_PLATES;
   
    double score = 1 - currentMonster()->getXMax()/(double)WIDTH_PLATES;
    unsigned int itemIndex = stacks[sequence[sequenceIndex + 1]].top();
    const std::vector<GlassLocation>& locations = currentMonster()->getLocationsForItemIndex(itemIndex);
    if (locations.empty()) { //std::cout << "locations empty for sequenceIndex#" << sequenceIndex << std::endl;
        return 1 - currentMonster()->getXMax()/(double)WIDTH_PLATES;}

    for (const GlassLocation& locationBis: locations) {        
        //std::cout << locationBis << std::endl;
        /*if (locationBis.getX() == 0 && locationBis.getY() == 0) { 
            std::cout << "=====================" << std::endl;
            std::cout << currentBinId << std::endl;
            displayLocations();
        }*/
        if (!attempt(locationBis)) continue;
        score = std::max(score, 1. + deepScore(sequenceIndex + 1, depth - 1));
        revert();
        if (score > 10) 
            return score;
    }
    assert (score < 10);
    //if (score > 15)
    //std::cout << "sequenceIndex#" << sequenceIndex << " -score: " << score << std::endl;
    return score;
}

unsigned int GlassCutter::getCurrentScore() {
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

void GlassCutter::displayStacks(){
    for (const GlassStack& stack: stacks) 
        std::cout << stack;
}

void GlassCutter::displayLocations() {
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
    //std::cout << "incrBinId" << std::endl;
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
        /*std::cout << "---|||---|||---" << std::endl;
        std::cout << e.what() << std::endl;
        displayLocations();*/
        nbInfeasible++;
        return false;
    }
}

// TODO build pour guillotine cut?
bool GlassCutter::attempt(const GlassLocation& location) {
    if (VERBOSE) std::cout << "attempt location: " << location << std::endl;
    setBinId(location.getBinId());
    nbAttempts++;
    stacks[location.getStackId()].pop();
    currentMonster()->incrRedMonster(location);
    currentLocations()->push_back(location);
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