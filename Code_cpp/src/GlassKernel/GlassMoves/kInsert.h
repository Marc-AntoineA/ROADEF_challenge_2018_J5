#pragma once

#include "../../GlassKernel/glassMove.h"

/*
    Déplace le bloc [fromIndex, fromIndex + 1, ..., fromIndex + k - 1]
    À la position [toIndex] et suivantes
*/

class KInsert: public GlassMove {
    public:
    KInsert(Heuristic* heuristic, unsigned int k);
    int attempt();
    void commit();
    void revert();

    private:
    unsigned int k;
    unsigned int fromIndex;
    unsigned int toIndex;
    bool doNothing() const;
};