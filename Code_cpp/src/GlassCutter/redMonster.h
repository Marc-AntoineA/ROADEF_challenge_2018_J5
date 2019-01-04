#pragma once

#include "redPoint.h"
#include "../GlassData/glassInstance.h"

#include <vector>

class RedMonster {
    public:
    RedMonster();
    void reset();
    void setInstance(GlassInstance* instance) { this->instance = instance; }
    void setPlateIndex(unsigned int plateIndex) { this->plateIndex = plateIndex; }

    private:
    GlassInstance* instance;
    unsigned int plateIndex;
    std::vector<RedPoint> points;
};