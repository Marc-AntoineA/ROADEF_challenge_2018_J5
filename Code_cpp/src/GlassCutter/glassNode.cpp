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
        //std::cout << (*this) << std::endl;
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

void GlassNode::preCheckTrimming(const GlassLocationIt& first, const GlassLocationIt& last) const {
    if (depth < 3) return;
        if (std::distance(first, last) > 1) throw std::runtime_error("Trimming failed (prechecked)");
}

void GlassNode::checkDimensions(const GlassLocationIt& first, const GlassLocationIt& last) const {
    if (std::distance(first, last) == 0) return;

    if (depth == 1) {
        if (width > MAX_XX) 
            throw std::runtime_error("1-cut failed");
        if (width < MIN_XX)
            throw std::runtime_error("1-cut failed");
        return;
    }
    if (depth == 2) {
        if (height < MIN_YY)
            throw std::runtime_error("1-cut failed");
    }
    if (depth == 3) {
        for (GlassLocationIt locationIt = first; locationIt != last; locationIt++) {
            if (locationIt->getX() != x || locationIt->getWidth() != width)
                throw std::runtime_error("Other mistake");
            if (locationIt->getY() != y && locationIt->getYH() != y + height)
                throw std::runtime_error("Other mistake");
        }
        return;
    } 

    if (depth == 2) {
        for (GlassLocationIt locationIt = first; locationIt != last; locationIt++) {
            if (locationIt->getY() != y && locationIt->getYH() != y + height)
                throw std::runtime_error("Other mistake");
        }
    }
}

bool GlassNode::isNodeFitLocation(const GlassLocation& location) const {
    assert(location.getBinId() == plateIndex);
    return x == location.getX() && y == location.getY() 
        && width == location.getWidth() && height == location.getHeight();
}

void GlassNode::buildCutsAvailable(const GlassLocationIt& first, const GlassLocationIt& last) {
    cutsAvailable.clear();
    unsigned int index = std::distance(first, last) - 1;
    for (GlassLocationIt locationIt = first; locationIt != last; locationIt++) {
        if (isVerticalCut()) {
            cutsAvailable.push_back(GlassCut(locationIt->getX(), index, !BEGIN_ITEM, VERTICAL_CUT));
            cutsAvailable.push_back(GlassCut(locationIt->getXW(), index, BEGIN_ITEM, VERTICAL_CUT));
        } else {
            cutsAvailable.push_back(GlassCut(locationIt->getY(), index, !BEGIN_ITEM, !VERTICAL_CUT));
            cutsAvailable.push_back(GlassCut(locationIt->getYH(), index, BEGIN_ITEM, !VERTICAL_CUT));
        }
        index--;
    }
    std::sort(cutsAvailable.begin(), cutsAvailable.end());
    //std::cout << "-----------------" << std::endl;
    //displayCutsAvailable();
}

bool GlassNode::isFreeOfDefects(const GlassCut& cut) const {
   if (cut.isVerticalCut())
        return getPlate().cutIsFreeOutOfDefects(cut.getAbscissa(), y, height, cut.isVerticalCut());
    else
        return getPlate().cutIsFreeOutOfDefects(cut.getAbscissa(), x, width, cut.isVerticalCut());
    return true;
}

bool GlassNode::isCutPossibleForMinXY(unsigned int prevAbscissa, unsigned int abscissa) {
    int delta = prevAbscissa - abscissa;
    if (delta <= 0) std::cout<< prevAbscissa << " < " << abscissa << std::endl;
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
    unsigned int firstAbscissa = isVerticalCut() ? x : y;
    unsigned int lastAbscissa = isVerticalCut() ? x + width : y + height;
    unsigned int prevAbscissa = lastAbscissa;
    unsigned int nbPrevItems = 0;
    
    realCuts.push_back(RealCut(prevAbscissa, nbItemsSeen));
    for (const GlassCut& cut: cutsAvailable) {
        //std::cout << prevAbscissa << " new cut " << cut.getAbscissa() << std::endl;

        if (!cut.isBegin()) { assert(openedCuts > 0); openedCuts--; }

        if (cut.getAbscissa() != firstAbscissa && cut.getAbscissa() != lastAbscissa
            && prevAbscissa - cut.getAbscissa() >= MIN_WASTE_AREA && openedCuts == 0
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
                if (realCuts.size() >= 2) {
                    throw std::runtime_error("Trimming failed (more than 1 cut)");
                }
                realCuts.push_back(RealCut(cut.getAbscissa(), nbItemsSeen));
                prevAbscissa = cut.getAbscissa();
                nbPrevItems = nbItemsSeen;
            }            
        }

        if (cut.isBegin()) {
            openedCuts++;
            maxItemSeen = std::max(maxItemSeen, (int)cut.getItemSequencePosition());
            nbItemsSeen++;
            // TODO warning ici...
        }
    }
    realCuts.push_back(RealCut(firstAbscissa, nbItemsSeen));
    /*std::cout << "====|===" << std::endl;
    displayRealCuts();*/
}

unsigned int GlassNode::cutRealCuts(const GlassLocationIt& first, unsigned int nbItemsToCuts) {
    unsigned int nbItemsCuts = 0;
    unsigned int prevAbscissa = isVerticalCut() ? x + width : y + height;
    unsigned int nbPrevItems = 0;
    
    for (const RealCut& cut: realCuts) {
        if (cut.x != prevAbscissa) {
            if (isVerticalCut())
                sons.push_back(GlassNode(instance, plateIndex, cut.x, y, prevAbscissa - cut.x, height, depth + 1, BRANCH));
            else
                sons.push_back(GlassNode(instance, plateIndex, x, cut.x, width, prevAbscissa - cut.x, depth + 1, BRANCH));
            //std::cout << cut.nbItems << " " << nbItemsToCuts - nbPrevItems << std::endl;
            nbItemsCuts += sons.back().buildNodeAndReturnNbItemsCuts(first + nbItemsToCuts - cut.nbItems, first + nbItemsToCuts - nbPrevItems);
            prevAbscissa = cut.x;
            nbPrevItems = cut.nbItems;
        }
    }
    
    unsigned int endNodeAbscissa = isVerticalCut() ? x : y;
    assert(prevAbscissa == endNodeAbscissa);
    if (prevAbscissa != endNodeAbscissa) {
        if (isVerticalCut())
            sons.push_back(GlassNode(instance, plateIndex, endNodeAbscissa, y, prevAbscissa, height, depth + 1, BRANCH));
        else
            sons.push_back(GlassNode(instance, plateIndex, x, endNodeAbscissa, width, prevAbscissa, depth + 1, BRANCH));
        nbItemsCuts += sons.back().buildNodeAndReturnNbItemsCuts(first, first + nbPrevItems);
    }
    return nbItemsCuts;
}

unsigned int GlassNode::buildNodeAndReturnNbItemsCuts(const GlassLocationIt& first, const GlassLocationIt& last) {
    this->firstItem = first;
    this->lastItem = last;
    sons.clear();
    checkTooSmall();
    
    if (first == last) {
        type = WASTE;
        return 0;
    }

    if (first + 1 == last && isNodeFitLocation(*first)) {
        setType(first->getItemIndex());
        return 1;
    }
    checkTooDepth();
    preCheckTrimming(first, last);
    checkDimensions(first, last);
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

void GlassNode::displayNode() const {
    displayNode(" ");
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
    std::reverse(sons.begin(), sons.end());
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

double GlassNode::getSurfaceOccupation() const {
    if (type == RESIDUAL) return 1;
    double itemsArea = 0.;
    for (GlassLocationIt it = firstItem; it < lastItem; it++) {
        itemsArea += instance->getItem(it->getItemIndex()).getArea();
    }
    return itemsArea/(width*height);
}

// WARNING: nodes are in reversed order
unsigned int GlassNode::getXMax() const {
    for (unsigned int index = 0; index < sons.size(); index++) {
        const GlassNode& son = sons[index]; 
        if (son.getNbItems() > 0) return son.getX() + son.getWidth();
    }
    return 0;
}