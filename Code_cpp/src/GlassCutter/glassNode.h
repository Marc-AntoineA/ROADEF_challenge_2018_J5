#pragma once

#include "../GlassData/glassInstance.h"

class GlassNode {

    public:

    GlassNode();
    void reset();
    void setPlateIndex(unsigned int plateIndex) { this->plateIndex = plateIndex; }
    void setInstance(GlassInstance* instance) { this->instance = instance; }

    private:
    unsigned int plateIndex;
    GlassInstance* instance;
};