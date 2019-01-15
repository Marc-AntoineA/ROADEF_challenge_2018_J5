#include "glassNode.h"

#include "../GlassData/glassConstants.h"

#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <iostream>

GlassNode::GlassNode(GlassInstance* instance, unsigned int plateIndex, 
    unsigned int x, unsigned int y, unsigned int width, unsigned int height,
    unsigned int depth, int type)
    : instance(instance), plateIndex(plateIndex), x(x), y(y), width(width), height(height), depth(depth), type(type), 
    sons(std::vector<GlassNode>()), cutsAvailable(std::vector<GlassCut>()), realCuts(std::vector<RealCut>()) {
}

GlassNode::GlassNode()
    : x(0), y(0), width(WIDTH_PLATES), height(HEIGHT_PLATES), depth(0), type(BRANCH),
    sons(std::vector<GlassNode>()), cutsAvailable(std::vector<GlassCut>()), realCuts(std::vector<RealCut>()) {
}

void GlassNode::reset() {
    setX(0);
    setY(0);
    setWidth(WIDTH_PLATES);
    setHeight(HEIGHT_PLATES);
    setDepth(0);
    setType(BRANCH);
    sons.clear();
    cutsAvailable.clear();
    realCuts.clear();
}

void GlassNode::checkTooSmall() const {
    if (width < MIN_WASTE_AREA || height < MIN_WASTE_AREA) {
        //std::cout << (*this) << std::endl;
        throw std::runtime_error("Waste too small");
    }
}

void GlassNode::checkTooDepth() const {
    if (depth > MAX_DEPTH)
        throw std::runtime_error("Tree too depth");
}

bool GlassNode::isNodeFitLocation(const GlassLocation& location) const {
    assert(location.getBinId() == plateIndex);
    return x == location.getX() && y == location.getY() 
        && width == location.getWidth() && height == location.getHeight();
}

void GlassNode::buildCutsAvailable(const GlassLocationIt& first, const GlassLocationIt& last) {
    cutsAvailable.clear();
    unsigned int index = 0;
    for (GlassLocationIt locationIt = first; locationIt != last; locationIt++) {
        if (isVerticalCut()) {
            cutsAvailable.push_back(GlassCut(locationIt->getX(), index, BEGIN_ITEM, VERTICAL_CUT));
            cutsAvailable.push_back(GlassCut(locationIt->getXW(), index, !BEGIN_ITEM, VERTICAL_CUT));
        } else {
            cutsAvailable.push_back(GlassCut(locationIt->getY(), index, BEGIN_ITEM, !VERTICAL_CUT));
            cutsAvailable.push_back(GlassCut(locationIt->getYH(), index, !BEGIN_ITEM, !VERTICAL_CUT));
        }
        index++;
    }
    std::sort(cutsAvailable.begin(), cutsAvailable.end());
}

bool GlassNode::isFreeOfDefects(const GlassCut& cut) const {
   if (cut.isVerticalCut())
        return getPlate().cutIsFreeOutOfDefects(cut.getAbscissa(), y, height, cut.isVerticalCut());
    else
        return getPlate().cutIsFreeOutOfDefects(cut.getAbscissa(), x, width, cut.isVerticalCut());
    return true;
}

bool GlassNode::isCutPossibleForMinXY(unsigned int prevAbscissa, unsigned int abscissa) {
    int delta = abscissa - prevAbscissa;
    assert(delta > 0);
    assert(depth < 3);
    switch (depth) {
        case 0:
            return delta > MIN_XX && delta < MAX_XX;
        case 1:
            return delta > MIN_YY;
        case 2:
            return true;
        assert(false);
        return false;
    }
    return false;
}

bool GlassNode::isCutPossibleForMinWaste(unsigned int abscissa) {
    for (const GlassCut& cut: cutsAvailable) {
        if (cut.getAbscissa() != abscissa && abs((int)cut.getAbscissa() - (int)abscissa) < MIN_WASTE_AREA) {
            return false;
        }
    }
    return true;
}

void GlassNode::buildRealCuts() {
    realCuts.clear();
    unsigned int openedCuts = 0;
    int maxItemSeen = -1;
    
    unsigned int nbItemsSeen = 0;
    unsigned int prevAbscissa = isVerticalCut() ? x : y;
    unsigned int nbPrevItems = 0;
    
    //realCuts.push_back(RealCut(prevAbscissa, nbItemsSeen));
    for (const GlassCut& cut: cutsAvailable) {
        if (!cut.isBegin()) { assert(openedCuts > 0); openedCuts--; }

        if (cut.getAbscissa() - prevAbscissa >= MIN_WASTE_AREA && openedCuts == 0
            && maxItemSeen + 1 == nbItemsSeen && isCutPossibleForMinWaste(cut.getAbscissa()) && isFreeOfDefects(cut)) {
            bool isNotWasteCut = nbItemsSeen > nbPrevItems;

            if (depth < 3) {
                if (isNotWasteCut) {
                    if (isCutPossibleForMinXY(prevAbscissa, cut.getAbscissa())) {
                        realCuts.push_back(RealCut(cut.getAbscissa(), nbItemsSeen));
                        prevAbscissa = cut.getAbscissa();
                        nbPrevItems = nbItemsSeen;
                    }
                } else {
                    realCuts.push_back(RealCut(cut.getAbscissa(), nbItemsSeen));
                    prevAbscissa = cut.getAbscissa();
                }
            } else {
                if (nbItemsSeen - nbPrevItems > 1) 
                    throw std::runtime_error("Trimming failed (more than 1 item)");
                if (realCuts.size() >= 1)
                    throw std::runtime_error("Trimming failed (more than 1 cut)");
                realCuts.push_back(RealCut(cut.getAbscissa(), nbItemsSeen));
                prevAbscissa = cut.getAbscissa();
                nbPrevItems = nbItemsSeen;
            }            
        }

        if (cut.isBegin()) {
            openedCuts++;
            maxItemSeen = std::max(maxItemSeen, (int) cut.getItemSequencePosition());
            nbItemsSeen++;
            // TODO warning ici...
        }
    }
    realCuts.push_back(RealCut(isVerticalCut() ? x + width: y + height, nbItemsSeen));
}

unsigned int GlassNode::cutRealCuts(const GlassLocationIt& first, unsigned int nbItemsToCuts) {
    unsigned int nbItemsCuts = 0;
    unsigned int prevAbscissa = isVerticalCut() ? x : y;
    unsigned int nbPrevItems = 0;
    
    for (const RealCut& cut: realCuts) {
        if (cut.x != prevAbscissa) {
            if (isVerticalCut())
                sons.push_back(GlassNode(instance, plateIndex, prevAbscissa, y, cut.x - prevAbscissa, height, depth + 1, BRANCH));
            else
                sons.push_back(GlassNode(instance, plateIndex, x, prevAbscissa, width, cut.x - prevAbscissa, depth + 1, BRANCH));
            nbItemsCuts += sons.back().buildNodeAndReturnNbItemsCuts(first + nbPrevItems, first + cut.nbItems);
            prevAbscissa = cut.x;
            nbPrevItems = cut.nbItems;
        }
    }
    
    unsigned int endNodeAbscissa = isVerticalCut() ? x + width : y + height;
    //assert(prevAbscissa == endNodeAbscissa);
    if (prevAbscissa != endNodeAbscissa) {
        if (isVerticalCut())
            sons.push_back(GlassNode(instance, plateIndex, prevAbscissa, y, endNodeAbscissa - prevAbscissa, height, depth + 1, BRANCH));
        else
            sons.push_back(GlassNode(instance, plateIndex, x, prevAbscissa, width, endNodeAbscissa - prevAbscissa, depth + 1, BRANCH));
        nbItemsCuts += sons.back().buildNodeAndReturnNbItemsCuts(first + nbPrevItems, first + nbItemsToCuts);
    }
    return nbItemsCuts;
}

unsigned int GlassNode::buildNodeAndReturnNbItemsCuts(const GlassLocationIt& first, const GlassLocationIt& last) {
    sons.clear();
    checkTooSmall();
    //std::cout << (*this) << std::endl;
    if (first == last) {
        type = WASTE;
        return 0;
    }

    if (first + 1 == last && isNodeFitLocation(*first)) {
        setType(first->getItemIndex());
        return 1;
    }

    checkTooDepth();
    buildCutsAvailable(first, last);
    //displayCutsAvailable();
    buildRealCuts();
    //displayRealCuts();
    unsigned int nbItemsCuts = cutRealCuts(first, std::distance(first, last));
    if (std::distance(first, last) != nbItemsCuts){
        throw std::runtime_error("Infeasible cut (not enough items cut)");
    }
    return nbItemsCuts;
}

void GlassNode::displayCutsAvailable() const {
    for (const GlassCut& cut: cutsAvailable) {
        std::cout << "cut. abscissa " << cut.getAbscissa();
        std::cout << " & begin " << cut.isBegin() << " & item " << cut.getItemSequencePosition() << std::endl;
    }
}

void GlassNode::displayRealCuts() const {
    for (const RealCut& cut: realCuts) {
        std::cout << "cut. abscissa " << cut.x << " & nbItems " << cut.nbItems << std::endl; 
    }
}

void GlassNode::displayNode(std::string prefix) const {
    std::cout << prefix << plateIndex << " " << x << " " << y << " " << x + width << " " << y + height << std::endl;
    for (const GlassNode& son: sons) {
        son.displayNode(prefix + " ");
    }
}

unsigned int GlassNode::saveNode(std::ofstream& outputFile, unsigned int nodeId, int parentId, bool last) {
    outputFile << plateIndex << ";" << nodeId << ";";
    outputFile << x << ";" << y << ";" << width << ";" << height << ";";
    outputFile << type << ";" << depth << ";";
    
    if (parentId < 0)
        outputFile << "" << std::endl;
    else
        outputFile << parentId << std::endl;

    if (last && !sons.back().hasSons())
        sons.back().setType(RESIDUAL);

    unsigned int maxSonId = nodeId;
    for (GlassNode& son: sons) {
        maxSonId = son.saveNode(outputFile, maxSonId + 1, nodeId, !LAST_BIN);
    }
    return maxSonId;
}

std::ostream& operator<<(std::ostream& os, const GlassNode& node) {
    os << node.getPlateIndex() << " " <<  node.getDepth() << " " << node.getX() << " " << node.getY() << " ";
    os << node.getX() + node.getWidth() << " " << node.getY() + node.getHeight();
    return os;
}