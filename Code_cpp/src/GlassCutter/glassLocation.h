#pragma once

#include "../GlassData/glassInstance.h"

class GlassLocation {
    public:
    GlassLocation(): itemIndex(0), binId(0), x(0), y(0), rotated(false), instance(NULL), locationSequence(0) {}
    
    GlassLocation(unsigned int itemIndex, unsigned int binId, unsigned int x, unsigned int y, bool rotated, 
        GlassInstance* instance, unsigned int locationSequence)
        : itemIndex(itemIndex), binId(binId), x(x), y(y), rotated(rotated),
         instance(instance), locationSequence(locationSequence){
    }

    unsigned int getX() const { return x; }
    unsigned int getY() const { return y; }
    unsigned int getXW() const { return x + getItem().getWidth(rotated); }
    unsigned int getYH() const { return y + getItem().getHeight(rotated); }
    unsigned int getBinId() const { return binId; }
    unsigned int getLocationSequence() const { return locationSequence; }
    unsigned int getItemIndex() const { return itemIndex; }
    bool getRotated() const { return rotated; }

    private:
    GlassItem& getItem() const { return instance->getItem(itemIndex); }

    unsigned int itemIndex;
    unsigned int binId;
    unsigned int x;
    unsigned int y;
    bool rotated;
    GlassInstance* instance;
    unsigned int locationSequence;
};

std::ostream& operator<<(std::ostream& os, const GlassLocation& location);