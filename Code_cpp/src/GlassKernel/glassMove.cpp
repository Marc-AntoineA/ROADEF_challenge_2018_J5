#include "glassMove.h"

#include "../GlassKernel/heuristic.h"

#include <iostream>

GlassMove::GlassMove(Heuristic* heuristic): heuristic(heuristic), nbImproved(0), nbAccepted(0), nbRefused(0), nbTested(0) {
    this->name = "name";
}

void GlassMove::addStat(MOVE_STATISTIC statistic) {
    switch (statistic) {
        case IMPROVE:
            nbAccepted++;
            nbImproved++;
            break;

        case ACCEPTED:
            nbAccepted++;
            break;

        case REFUSED:
            nbRefused++;
            break;
    }
}

void GlassMove::addTested() {
    nbTested++;
}

void GlassMove::displayStatistics() {
    std::cout << "Move. " << name << ": ";  
    std::cout << nbImproved << " | " << nbAccepted;
    std::cout << " | " << nbRefused << " (" << nbTested << ")" << std::endl;  
}

unsigned int GlassMove::getSequenceSize() const {
    return heuristic->getSequence().size();
}