#include "glassCutter.h"
#include "../GlassData/glassConstants.h"


GlassCutter::GlassCutter(GlassInstance* instance): instance(instance) {
    nodes.resize(NB_PLATES);
    monsters.resize(NB_PLATES);
    currentBinId = 0;

    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
        nodes[plateIndex].setPlateIndex(plateIndex);
        nodes[plateIndex].setInstance(instance);
        monsters[plateIndex].setPlateIndex(plateIndex);
        monsters[plateIndex].setInstance(instance);
    }
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