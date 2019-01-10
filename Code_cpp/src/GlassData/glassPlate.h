#pragma once

#include "glassDefect.h"

#include <vector>

class GlassPlate {

    public:
    GlassPlate(){} ;
    void setPlateId(unsigned int plateId) { this->plateId = plateId; }
    unsigned int getPlateId() { return plateId; }
    void addDefect(GlassDefect defect);
    bool rectangleIsFreeOutOfDefects(unsigned int x, unsigned int y, unsigned int xw, unsigned int yh) const;
    unsigned int getBestY(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const;
    const std::vector<GlassDefect>& getDefects() const { return defects; }
    void sortDefects();
    
    private:
    unsigned int plateId;
    std::vector<GlassDefect> defects;
};