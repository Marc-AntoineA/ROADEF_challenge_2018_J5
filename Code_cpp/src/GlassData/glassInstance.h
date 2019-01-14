#pragma once

#include "glassItem.h"
#include "glassPlate.h"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

class GlassInstance {
    public:
    GlassInstance (std::string instancePath);
    unsigned int getNbItems() const { return items.size(); }
    const GlassItem& getItem(unsigned int itemIndex) { assert(itemIndex < items.size()); return items[itemIndex]; }
    std::string& getInstancePath() { return instancePath; }
    unsigned int getItemsArea() { return itemsArea; }
    GlassPlate& getPlate(unsigned int plateIndex) { return plates[plateIndex]; }
    std::vector<GlassItem>& getItems() { return items; }

    private:
    void computeItemsArea();
    void buildItems();
    void initPlates();
    void buildPlates();

    unsigned int itemsArea;
    std::vector<GlassItem> items;
    std::vector<GlassPlate> plates;
    std::string instancePath;
};