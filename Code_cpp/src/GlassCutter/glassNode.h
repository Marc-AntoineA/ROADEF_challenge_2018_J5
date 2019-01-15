#pragma once

#include "../GlassData/glassInstance.h"
#include "../GlassCutter/glassLocation.h"
#include "../GlassCutter/glassCut.h"

#define BRANCH -2
#define WASTE -1
#define RESIDUAL -3
#define LAST_BIN true

struct RealCut {
    unsigned int x;
    unsigned int nbItems;
    
    RealCut(unsigned int x, unsigned int nbItems) : x(x), nbItems(nbItems) {}
};

using GlassLocationIt = std::vector<GlassLocation>::iterator;

class GlassNode {

    public:

    GlassNode();
    GlassNode(GlassInstance* instance, unsigned int plateIndex, 
        unsigned int x, unsigned int y, unsigned int width, unsigned int height,
        unsigned int depth, int type);
    void setPlateIndex(unsigned int plateIndex) { this->plateIndex = plateIndex; }
    unsigned int getPlateIndex() const { return plateIndex; }
    void setInstance(GlassInstance* instance) { this->instance = instance; }
    void setX(unsigned int x) { this->x = x; }
    unsigned int getX() const { return x; }
    void setY(unsigned int y) { this->y = y; }
    unsigned int getY() const { return y; }
    void setWidth(unsigned int width) { this->width = width; }
    unsigned int getWidth() const { return width; }
    void setHeight(unsigned int height) { this->height = height; }
    unsigned int getHeight() const { return height; }
    void setDepth(unsigned int depth) { this->depth = depth; }
    unsigned int getDepth() const { return depth; }
    void setType(int type) { this->type = type; }
    unsigned int getType() const { return type; }
    bool hasSons() const { return !sons.empty(); }

    void reset();
    void displayNode(std::string prefix) const;
    unsigned int buildNodeAndReturnNbItemsCuts(const GlassLocationIt& first, const GlassLocationIt& last);    
    void checkNode(const GlassLocationIt& first, const GlassLocationIt& last);
    unsigned int saveNode(std::ofstream& outputFile, unsigned int nodeId, int parentId, bool last);

    private:
    void buildCutsAvailable(const GlassLocationIt& first, const GlassLocationIt& last);
    void buildRealCuts();
    unsigned int cutRealCuts(const GlassLocationIt& first, unsigned int nbItemsToCuts);
    
    void displayCutsAvailable() const;
    void displayRealCuts() const;

    bool isFreeOfDefects(const GlassCut& cut) const;
    bool isCutPossibleForMinXY(unsigned int prevAbscissa, unsigned int abscissa);
    bool isCutPossibleForMinWaste(unsigned int abscissa);
    bool isVerticalCut() const { return depth%2 == 0; }
    void checkTooSmall() const;
    void checkTooDepth() const;
    bool isNodeFitLocation(const GlassLocation& location) const;
    
    GlassPlate& getPlate() const { return instance->getPlate(plateIndex); }

    GlassInstance* instance;
    unsigned int plateIndex;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    int type;
    std::vector<GlassNode> sons;
    std::vector<GlassCut> cutsAvailable;
    std::vector<RealCut> realCuts;
};

std::ostream& operator<<(std::ostream& os, const GlassNode& node);