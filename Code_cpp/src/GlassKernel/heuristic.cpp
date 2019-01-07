#include "heuristic.h"

#include "../GlassCutter/glassCutter.h"
#include "glassRand.h"

#include <iostream>

Heuristic::Heuristic(GlassInstance instance): instance(instance), cutter(&instance, sequence){
    initRandomlySequence();
    displaySequence();
    cutter.setSequence(sequence);
    cutter.cut();
}

void Heuristic::initRandomlySequence() {
    sequence.clear();
    unsigned int nbItems = instance.getNbItems();

    std::vector<bool> itemsSelected;
    itemsSelected.resize(nbItems);
    for (unsigned int i = 0; i < itemsSelected.size(); i++) 
        itemsSelected[i] = false;

    unsigned int nbItemsSelected = 0;
    while (nbItemsSelected < nbItems) {
        unsigned int itemIndex = randint(0, nbItems);
        if (itemsSelected[itemIndex]) continue;
        sequence.push_back(instance.getItem(itemIndex).getStackId());
        itemsSelected[itemIndex] = true;
        nbItemsSelected++;
    }
}

void Heuristic::displaySequence() {
    if (sequence.size() == 0) {
        std::cout << "Sequence courante vide" << std::endl;
        return;
    }

    std::cout << "Sequence courante : [";
    for (unsigned int index = 0; index < sequence.size() - 1; index++)
        std::cout << sequence[index] << ", ";
    std::cout << sequence[sequence.size() - 1] << "]" << std::endl;
}

unsigned int Heuristic::computeScore() {
    cutter.setSequence(sequence); // TODO a priori inutile
    cutter.cut();
    return cutter.getCurrentScore();
}