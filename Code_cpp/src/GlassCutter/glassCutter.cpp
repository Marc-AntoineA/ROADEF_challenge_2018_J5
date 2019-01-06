#include "glassCutter.h"
#include "../GlassData/glassConstants.h"


GlassCutter::GlassCutter(GlassInstance* instance): instance(instance) {
    currentBinId = 0;
    buildStacks();
    buildMonsters();
    buildNodes();
}

void GlassCutter::reset() {
    currentBinId = 0;
    for (GlassNode& node: nodes) 
        node.reset();
    for (RedMonster& monster: monsters)
        monster.reset();
}

void GlassCutter::initWithSequence(const std::vector<unsigned int>& sequence){
    // todo
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