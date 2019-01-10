#include "glassMove.h"

#include <iostream>

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