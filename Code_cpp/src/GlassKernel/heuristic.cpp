#include "heuristic.h"

#include "../GlassCutter/glassCutter.h"
#include "../GlassKernel/glassMove.h"
#include "../GlassData/glassConstants.h"
#include "../GlassKernel/GlassMoves/swap.h"
#include "../GlassKernel/GlassMoves/kConsecutivePermutation.h"
#include "../GlassKernel/GlassMoves/kPermutation.h"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/thread.hpp>

using namespace boost::chrono;
typedef duration<double> sec;

boost::mutex ioMutex;

Heuristic::Heuristic(GlassInstance* instance, unsigned int timeLimit, unsigned int depthLimit, unsigned int seed)
    :instance(instance), cutter(instance, sequence), 
    timeLimit(timeLimit), begin(clock()), depthLimit(depthLimit), nbIterations(0), gen(seed) {
}

void Heuristic::start() {
    bestScore = WIDTH_PLATES*HEIGHT_PLATES*NB_PLATES;
    begin = systemClock.now().time_since_epoch();
    initRandomlySequence();
    displaySequence();
    buildMoves();
    //initSeainitSearch();
    //localSearch(depthLimit, timeLimit);
    displayMoveStatistics();
    computeScore(depthLimit);
    cutter.displayStatistics();
}

void Heuristic::initSearch() {
    localSearch(1, 10);
    cutter.reset();
}

unsigned int Heuristic::getCurrentDurationOnSeconds() const {
    sec duration = systemClock.now().time_since_epoch() - begin;
    return (unsigned int)(round(duration.count()));
}

unsigned int Heuristic::glassRandint(unsigned int begin, unsigned int last) {
    boost::uniform_int<> dist(begin, last - 1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
    return die();
}

void Heuristic::buildMoves() {
    poolMoves.clear();
    poolMoves.push_back(new Swap(this));
    for (unsigned int k = 2; k < std::min((std::size_t)6, sequence.size()); k++) 
        poolMoves.push_back(new KConsecutivePermutation(this, k));
    for (unsigned int k = 3; k < std::min((std::size_t)4, sequence.size()); k++) 
        poolMoves.push_back(new KPermutation(this, k));
}

void Heuristic::initRandomlySequence() {
    sequence.clear();
    /*std::vector<unsigned int> sequenceItems = {23, 22,24,38, 18, 19, 3, 1, 12, 67, 68, 69, 48};//, 21, 32, 66,20,60,37,31,61,55,57,70,29,10,5, 13, 30, 43, 64, 59, 53, 51, 33, 35};
    //std::vector<unsigned int> sequenceItems = {0,10,1,2,13,3,9,16,8,14,15,4,5,11,6,12,7};
    for (unsigned int itemIndex: sequenceItems){
        unsigned int stackId = instance->getItem(itemIndex).getStackId();
        sequence.push_back(stackId);
    }
    //sequence = { 39, 69, 9, 24, 64, 1, 31, 12, 49, 35, 68, 45, 21, 22, 33, 44, 52, 16, 37, 55, 26, 70, 62, 15, 19, 60, 66, 5, 46, 11, 48, 34, 7, 63, 32, 40, 14, 67, 36, 29, 43, 20, 54, 4, 56, 17, 51, 65, 18, 41, 38, 2, 71, 57, 23, 0, 28, 53, 30, 61, 13, 42, 50, 8, 47, 10, 59, 6, 27, 58, 3, 25};
    return;*/

    unsigned int nbItems = instance->getNbItems();
    std::vector<bool> itemsSelected;
    itemsSelected.resize(nbItems);
    for (unsigned int i = 0; i < itemsSelected.size(); i++) 
        itemsSelected[i] = false;

    unsigned int nbItemsSelected = 0;
    while (nbItemsSelected < nbItems) {
        unsigned int itemIndex = glassRandint(0, nbItems);
        if (itemsSelected[itemIndex]) continue;
        sequence.push_back(instance->getItem(itemIndex).getStackId());
        itemsSelected[itemIndex] = true;
        nbItemsSelected++;
    }
    assert(sequence.size() == instance->getNbItems());
    //displaySequence();
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
    return computeScore(depth, 0);
}

unsigned int Heuristic::computeScore(unsigned int depth, unsigned int beginSequenceIndex) {
    cutter.setSequence(sequence); // TODO a priori inutile
    cutter.revertPlatesUntilSequenceIndex(beginSequenceIndex);
    cutter.cut(depth);
    return cutter.getCurrentScore();
}

void Heuristic::localSearch(unsigned int depth, unsigned int currentTimeLimit) {
    bestScore = computeScore(depth);
    nbIterations = 0;
    int beginSequenceIndex = 0;

    unsigned int previousDuration = 0;
    while (getCurrentDurationOnSeconds() < currentTimeLimit) {
        unsigned int moveIndex = glassRandint(0, poolMoves.size());
        GlassMove* move = poolMoves[moveIndex];
        int startingFrom = move->attempt();
        if (startingFrom == NOTHING) continue;
        nbIterations++;
        unsigned int score = computeScore(depth, (unsigned int) std::min(beginSequenceIndex, startingFrom));
        if (score <= bestScore) {
            move->commit();
            move->addStat(score < bestScore ? IMPROVE : ACCEPTED);
            //std::cout << " Nouveau score " << score << " vs " << bestScore << std::endl;
            bestScore = score;
            beginSequenceIndex = sequence.size();
        } else {
            move->revert();
            move->addStat(REFUSED);
            beginSequenceIndex = std::min(beginSequenceIndex, startingFrom);
        }

        if (getCurrentDurationOnSeconds() > 1 + previousDuration) {
            previousDuration = getCurrentDurationOnSeconds();
            displayLog();
        } 
    }
    /*displaySequence();
    cutter.displayLocations();
    cutter.displayErrorStatistics();
    std::cout << "Best score " << bestScore << std::endl;
    std::cout << "Instance area : " << instance->getItemsArea() << std::endl;
    std::cout << "Time elapsed :" << double((clock() - begin)) / CLOCKS_PER_SEC << "s" << std::endl;*/
}

void Heuristic::displayLog() const {
    boost::mutex::scoped_lock scopedLock(ioMutex);
    std::cout << boost::this_thread::get_id() << "\t" << getCurrentDurationOnSeconds() << "s\t" << bestScore << "\t" << nbIterations << std::endl;
}

void Heuristic::displayMoveStatistics() {
    for (GlassMove* move: poolMoves) {
        move->displayStatistics();
    }
}

void Heuristic::saveBest(std::string name) {
    cutter.saveBest(name);
}

void Heuristic::displayLocations() const {
    cutter.displayLocations();
}