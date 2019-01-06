#include "../GlassCutter/glassCutter.h"
#include "../GlassData/glassConstants.h"


GlassCutter::GlassCutter(GlassInstance* instance): instance(instance) {
    currentBinId = 0;
    nbRollbacks = 0;
    nbAttempts = 0;
    buildStacks();
    buildMonsters();
    buildNodes();
    buildLocations();
}

void GlassCutter::reset() {
    currentBinId = 0;
    for (GlassNode& node: nodes) 
        node.reset();
    for (RedMonster& monster: monsters)
        monster.reset();
}

void GlassCutter::initWithSequence(const std::vector<unsigned int>& sequence){
    if (VERBOSE) std::cout << "InitWithSequence en cours..." << std::endl;
    for (unsigned int stackId: sequence) {
        if (VERBOSE) std::cout << "StackId#" << stackId;
        unsigned int itemIndex = stacks[stackId].top();
        if (VERBOSE) std::cout << " & item#" << itemIndex << std::endl;
        std::vector<GlassLocation> locations = currentMonster()->getLocationsForItemIndex(itemIndex);
        while (locations.empty()) {
            incrBinId();
            locations = currentMonster()->getLocationsForItemIndex(itemIndex);
        }
        attempt(locations[0]); // todo remove
        /*for (const GlassLocation& location: locations) {
            std::cout << "location" << location << std::endl;
            if (attempt(location)) {
                std::cout << "attempt done" << std::endl;
                revert();
            }
        }*/
    }
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
}

// TODO build pour guillotine cut?
bool GlassCutter::attempt(const GlassLocation& location) {
    if (VERBOSE) std::cout << "attempt location: " << location << std::endl;
    nbAttempts++;
    stacks[location.getStackId()].pop();
    currentMonster()->incrRedMonster(location);
    currentLocations()->push_back(location);
    return true;
}

void GlassCutter::revert() {
    nbRollbacks++;
    while (currentLocations()->empty()) {
        if (currentBinId == 0) throw std::runtime_error("Rollback impossible (not enough items).");
        decrBinId();
    }

    unsigned int stackId = currentLocations()->back().getStackId();
    std::cout << stacks[stackId];
    stacks[stackId].push();
    currentLocations()->pop_back();
    currentMonster()->revert();
    if (currentLocations()->empty()) 
        decrBinId();    
}