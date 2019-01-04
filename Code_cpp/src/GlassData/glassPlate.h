#pragma once

#include "glassDefect.h"

#include <vector>

class GlassPlate {

    public:
    GlassPlate(){} ;
    void setPlateId(unsigned int plateId) { this->plateId = plateId; }
    unsigned int getPlateId() { return plateId; }
    void addDefect(GlassDefect defect);

    private:
    unsigned int plateId;
    std::vector<GlassDefect> defects;
};