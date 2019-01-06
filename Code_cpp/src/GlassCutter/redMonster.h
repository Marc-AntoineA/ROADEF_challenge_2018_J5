#pragma once

#include "../GlassCutter/redPoint.h"
#include "../GlassData/glassInstance.h"
#include "../GlassCutter/glassLocation.h"

#include <vector>
#include <queue>

class RedMonster {
    public:
    RedMonster();
    void reset();
    void setInstance(GlassInstance* instance) { this->instance = instance; }
    void setPlateIndex(unsigned int plateIndex) { this->plateIndex = plateIndex; }
    void incrRedMonster(const GlassLocation& location);
    void revert();
    std::vector<GlassLocation> getLocationsForItemIndex(unsigned int itemIndex);

    private:
    void incrGreenStar(const RedPoint& point);
    void killRedPointByIndex(unsigned int index);
    void cleanLocation(const GlassLocation& location);
    bool isFeasibleLocation(const GlassLocation& location);
    void addLocationsFreeOfDefectsForLocation(const GlassLocation& location, std::vector<GlassLocation>& locations);
    void reviveRedPoint(RedPoint& point);
    unsigned int getXMax();

    GlassInstance* instance;
    unsigned int plateIndex;
    std::vector<RedPoint> points;
    std::priority_queue<RedPoint, std::vector<RedPoint>, sortRedPointsByDeathTime> previousPoints;
    unsigned int time;
};