#include "heuristic.h"

#include "../GlassCutter/glassCutter.h"
#include "../GlassKernel/glassMove.h"
#include "../GlassData/glassConstants.h"
#include "../GlassKernel/GlassMoves/swap.h"
#include "../GlassKernel/GlassMoves/kConsecutivePermutation.h"
#include "../GlassKernel/GlassMoves/kPermutation.h"
#include "../GlassKernel/GlassMoves/kInsert.h"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/thread.hpp>

#define VERBOSE_HEURISTIC true

using namespace boost::chrono;
typedef duration<double> sec;

boost::mutex ioMutex;

Heuristic::Heuristic(GlassInstance* instance, unsigned int timeLimit, unsigned int depthLimit, unsigned int seed)
    :instance(instance), cutter(instance, sequence), 
    timeLimit(timeLimit), begin(clock()), depthLimit(depthLimit), 
    nbIterations(0), gen(seed), platesOccupation(std::vector<double>()){

    initPlatesOccupation();
}

void Heuristic::start() {
    bestScore = WIDTH_PLATES*HEIGHT_PLATES*NB_PLATES;
    begin = systemClock.now().time_since_epoch();
    initRandomlySequence();
    //displaySequence();
    buildMoves();
    //initSearch();
    localSearch(depthLimit, timeLimit);
    if(VERBOSE_HEURISTIC) displayMoveStatistics();
    computeScore(depthLimit);
    if(VERBOSE_HEURISTIC) cutter.displayStatistics();
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
    for (unsigned int k = 3; k < std::min((std::size_t)8, sequence.size()); k++) 
        poolMoves.push_back(new KConsecutivePermutation(this, k));
    for (unsigned int k = 3; k < std::min((std::size_t)6, sequence.size()); k++) 
        poolMoves.push_back(new KPermutation(this, k));
    for (unsigned int k = 1; k < std::min((std::size_t)4, sequence.size() - 1); k++)
        poolMoves.push_back(new KInsert(this, k));
}

void Heuristic::initRandomlySequence() {
    sequence.clear();
    // B13
    //sequence = { 17, 16, 5, 4, 11, 0, 5, 13, 6, 2, 8, 12, 15, 4, 3, 10, 4, 3, 4, 7, 13, 17, 5, 5, 9, 1, 10, 2, 3, 13, 15, 2, 0, 14, 0, 3, 13, 2, 13, 6, 11, 7, 15, 10, 8, 9, 14, 4, 2, 9, 12, 10, 1, 13, 7, 12, 7, 10, 0, 5, 4, 9, 17, 0, 8, 11, 13, 2, 5, 5, 7, 13, 8, 5, 6, 0, 9, 16, 17, 11, 9, 17, 1, 10, 6, 16, 2, 15, 3, 7, 17, 13, 2, 4, 11, 11, 5, 16, 10, 12, 3, 2, 9, 4, 9, 5, 11, 8, 12, 0, 15, 0, 1, 2, 12, 1, 2, 15, 7, 8, 11, 9, 1, 9, 0, 4, 1, 8, 6, 7, 4, 1, 16, 16, 9, 1, 1, 8, 6, 13, 6, 4, 7, 1, 9, 17, 4, 6, 15, 17, 11, 5, 9, 11, 10, 9, 7, 11, 11, 13, 2, 10, 6, 6, 12, 1, 6, 15, 3, 8, 14, 9, 5, 7, 5, 12, 17, 2, 9, 15, 9, 9, 10, 11, 0, 13, 0, 11, 6, 14, 5, 5, 11, 1, 11, 0, 7, 0, 11, 11, 0, 13, 9, 11, 16, 0, 10, 6, 3, 12, 5, 11, 10, 6, 8, 4, 8, 2, 5, 3, 13, 12, 3, 13, 10, 9, 1, 12, 14, 6, 0, 2, 4, 4, 4, 10, 12, 15, 8, 3, 12, 15, 16, 7, 8, 4, 13, 0, 3, 8, 3, 14, 7, 11, 8, 12, 4, 10, 3, 1, 16, 12, 17, 2, 7, 11, 5, 0, 11, 2, 3, 5, 2, 10, 1, 9, 11, 14, 6, 5, 6, 7, 0, 16, 6, 11, 12, 1, 1, 5, 14, 8, 7, 13, 8, 12, 13, 15, 9, 11, 13, 15, 15, 5, 8, 14, 9, 6, 12, 15, 5, 12, 16, 6, 7, 3, 3, 8, 3, 7, 10, 0, 5, 10, 5, 14, 10, 0, 12, 17, 16, 13, 11, 6, 9, 17, 9, 7, 1, 8, 12, 2, 1, 0, 0, 1, 10, 3, 0, 3, 4, 11, 13, 7, 13, 11, 11, 10, 14, 8, 17, 11, 9, 9, 3, 13, 3, 3, 8, 11, 5, 13, 11, 9, 6, 6, 6, 4, 11, 13, 7, 0, 7, 1, 5, 11, 16, 14, 1, 8, 11, 4, 2, 17, 12, 12, 7, 8, 4, 16, 11, 10, 13, 11, 4, 13, 14, 6, 2, 8, 5, 4, 3, 14, 6, 4, 3, 5, 13, 16, 5, 15, 8, 3, 10, 8, 2, 13, 8, 3, 4, 10, 7, 17, 2, 6, 9, 0, 3, 11, 0, 4, 7, 12, 12, 6, 5, 1, 1, 3, 0, 1, 2, 2, 10, 14, 11, 3, 3, 14, 5, 0, 12, 12, 1, 0, 2, 6, 0, 10, 6, 15, 7, 7, 5, 0, 10, 10, 10, 1, 8, 7, 2, 3, 3, 3, 12, 8, 8, 15, 6, 12, 2, 12, 0, 15, 1, 15, 10, 11, 1, 9, 3, 4, 4, 14, 7, 3, 6, 5, 2, 9, 11, 16, 4, 3, 5, 9, 16, 3, 14, 6, 2, 16, 11, 0, 2, 11, 11, 4, 10, 0, 4, 1, 9, 8, 8, 12, 9, 1, 0, 12, 12, 7, 13, 11, 2, 2, 12, 4, 10, 13, 0, 12, 7, 10, 11, 12, 7, 8, 14, 7, 6, 17, 6, 5, 4, 10, 9, 4, 10, 6, 1, 11, 4, 17, 9, 9, 7, 8, 1, 13, 14, 9, 2, 13, 2, 6, 1, 4, 3, 0, 8, 6, 6, 5, 2, 0, 1, 3, 9, 5, 12, 0, 11, 11, 0, 7, 4, 11, 7, 7, 8, 9, 15, 2, 11, 4, 10, 17, 10, 1, 12, 0, 2, 3, 2, 7, 5, 8, 3, 5, 8, 8, 6, 12, 11, 1, 9, 13, 10, 8, 1, 12, 8, 9, 1, 2, 16, 10, 16, 1, 4, 11, 7, 10};
    // A2
    //std::vector<unsigned int> sequenceItems = {23, 22,24,38, 18, 19, 3, 1, 12, 67, 68, 69, 48};//, 21, 32, 66,20,60,37,31,61,55,57,70,29,10,5, 13, 30, 43, 64, 59, 53, 51, 33, 35};
    // A20
    //std::vector<unsigned int> sequenceItems = {0,10,1,2,13,3,9,16,8,14,15,4, 5,11,6,12,7};
    // A17
    //std::vector<unsigned int> sequenceItems = {0, 19, 1, 2, 3, 4, 5, 20, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    /*for (unsigned int itemIndex: sequenceItems){
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


MOVE_STATISTIC Heuristic::evaluateCurrentSolution(unsigned int depth, 
    unsigned int beginSequenceIndex, unsigned int endSequenceIndex) {

    unsigned int currentScore = computeScore(depth, beginSequenceIndex, endSequenceIndex);
    if (currentScore > bestScore)
        return REFUSED;

    if (currentScore < bestScore) 
        return IMPROVE;

    if (currentScore == bestScore) {
        //return ACCEPTED;
        for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
            double currentSurfaceOccupation = cutter.getSurfacePlateOccupation(plateIndex);
            if (currentSurfaceOccupation < platesOccupation[plateIndex])
                return REFUSED;
            if (currentSurfaceOccupation > platesOccupation[plateIndex])
                return IMPROVE;
        }
    }
    return ACCEPTED;
}

void Heuristic::updateBestScore() {
    bestScore = cutter.getCurrentScore();
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
        platesOccupation[plateIndex] = cutter.getSurfacePlateOccupation(plateIndex);
    }
}

unsigned int Heuristic::computeScore(unsigned int depth) {
    return computeScore(depth, 0, sequence.size() + 1);
}

unsigned int Heuristic::computeScore(unsigned int depth, 
    unsigned int beginSequenceIndex, unsigned int endSequenceIndex) {
    cutter.setSequence(sequence); // TODO a priori inutile
    cutter.revertPlatesUntilSequenceIndex(beginSequenceIndex);
    if (cutter.cut(depth, endSequenceIndex)) 
        return cutter.getCurrentScore();
    return NB_PLATES*WIDTH_PLATES*HEIGHT_PLATES;
}

void Heuristic::localSearch(unsigned int depth, unsigned int currentTimeLimit) {
    bestScore = computeScore(depth);
    nbIterations = 0;
    unsigned int beginModifiedSequenceIndex = 0;
    unsigned int endModifiedSequenceIndex = sequence.size() - 1;

    unsigned int previousDuration = 0;
    while (getCurrentDurationOnSeconds() < currentTimeLimit) {
        unsigned int moveIndex = glassRandint(0, poolMoves.size());
        GlassMove* move = poolMoves[moveIndex];
        unsigned int startingFrom = 0;
        unsigned int endingTo = 0;
        move->attempt(startingFrom, endingTo);
        if (startingFrom == endingTo);
        beginModifiedSequenceIndex = std::min(beginModifiedSequenceIndex, startingFrom);
        // ending index deactivated
        endModifiedSequenceIndex = sequence.size();//std::max(endModifiedSequenceIndex, endingTo) + 2;
        nbIterations++;
        MOVE_STATISTIC result = evaluateCurrentSolution(depth, beginModifiedSequenceIndex, endModifiedSequenceIndex);
        move->addStat(result);

        if (result == IMPROVE)
            updateBestScore();

        if (result == ACCEPTED || result == IMPROVE) {
            move->commit();
            cutter.commitFirstIndexInEachPlate();
            beginModifiedSequenceIndex = sequence.size();
            endModifiedSequenceIndex = 0;
        } else {
            move->revert();
            move->addStat(REFUSED);
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
    if (!VERBOSE_HEURISTIC) return;
    boost::mutex::scoped_lock scopedLock(ioMutex);
    std::cout << boost::this_thread::get_id() << "\t" << getCurrentDurationOnSeconds() << "s\t" << bestScore << "\t" << nbIterations << std::endl;
    
    for (double occupation: platesOccupation) {
        if (occupation == 0) break;
        std::cout << "\t" << occupation;
    }
    std::cout << std::endl;
}

void Heuristic::displayMoveStatistics() {
    for (GlassMove* move: poolMoves) {
        move->displayStatistics();
    }
}

void Heuristic::saveBest(std::string name) {
    cutter.saveBest(name);
}

void Heuristic::saveAndComputeBest(std::string name) {
    cutter.reset();
    cutter.cut(depthLimit, sequence.size());
    cutter.saveBest(name);
}

void Heuristic::displayLocations() const {
    cutter.displayLocations();
}

void Heuristic::initPlatesOccupation() {
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++)
        platesOccupation.push_back(0);
}