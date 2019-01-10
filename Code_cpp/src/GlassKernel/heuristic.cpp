#include "heuristic.h"

#include "../GlassCutter/glassCutter.h"
#include "../GlassKernel/glassMove.h"
#include "../GlassData/glassConstants.h"
#include "../GlassKernel/GlassMoves/swap.h"
#include "../GlassKernel/GlassMoves/kConsecutivePermutation.h"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>

unsigned int Heuristic::glassRandint(unsigned int begin, unsigned int last) {
    return rand() % (last - begin) + begin;
}

Heuristic::Heuristic(GlassInstance instance): instance(instance), cutter(&instance, sequence){
    initRandomlySequence();
    buildMoves();
    displaySequence();
    localSearch(5);
    displayMoveStatistics();
    computeScore(5);
}

void Heuristic::buildMoves() {
    poolMoves.clear();
    poolMoves.push_back(new Swap(this));
    for (unsigned int k = 2; k <= 4; k++) 
        poolMoves.push_back(new KConsecutivePermutation(this, k));
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
        unsigned int itemIndex = glassRandint(0, nbItems);
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


unsigned int Heuristic::computeScore(unsigned int depth) {
    std::cout << "compute score for sequence : ";
    displaySequence();
    cutter.setSequence(sequence); // TODO a priori inutile
    cutter.reset();
    cutter.cut();
    return cutter.getCurrentScore();
}

void Heuristic::localSearch(unsigned int depth) {
    bestScore = computeScore(depth);
    for (unsigned int k = 0; k < 1000; k++) {
        unsigned int moveIndex = glassRandint(0, poolMoves.size());
        GlassMove* move = poolMoves[moveIndex];
        if(!move->attempt()) continue;
        displaySequence();
        unsigned int score = computeScore(depth);
        if (score <= bestScore) {
            move->commit();
            move->addStat(score < bestScore ? IMPROVE : ACCEPTED);
            std::cout << " Nouveau score " << score << " vs " << bestScore << std::endl;
            bestScore = score;
        } else {
            move->revert();
            move->addStat(REFUSED);
        }
    }
    std::cout << " Best score " << bestScore << std::endl;
}

void Heuristic::displayMoveStatistics() {
    for (GlassMove* move: poolMoves) {
        move->displayStatistics();
    }
}

void Heuristic::saveBest(std::string name) {
    cutter.saveBest(name);
}