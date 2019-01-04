#pragma once

#include "glassItem.h"
#include "glassPlate.h"

#include <string>
#include <vector>

class GlassInstance {
    public:
    GlassInstance (std::string instancePath);
    GlassInstance() {}
    unsigned int getNbItems() const { return items.size(); }
    GlassItem& getItem(unsigned int itemIndex) { return items[itemIndex]; }
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