#include "redMonster.h"

#include "../GlassData/glassConstants.h"

#include <cassert>

RedMonster::RedMonster() {
    time = 0;
}

void RedMonster::reset() {
    time = 0;
    points.clear();
    previousPoints = std::priority_queue<RedPoint, std::vector<RedPoint>, sortRedPointsByDeathTime>();
}

void RedMonster::incrRedMonster(const GlassLocation& location) {
    time++;
    cleanLocation(location);
    incrGreenStar(RedPoint(location.getXW(), location.getYH(), time));
}

void RedMonster::incrGreenStar(const RedPoint& point) {
    unsigned int pointIndex = 0;
    while (pointIndex < points.size()) {
        const RedPoint& rp = points[pointIndex];
        
        if (rp.getX() > point.getX()) 
            break;
        
        if (rp.getX() == point.getX() || rp.getY() <= point.getY()){
            killRedPointByIndex(pointIndex);
            continue;
        }
        pointIndex++;
    }

    if (pointIndex == points.size()) {
        points.push_back(point);
        return;
    }
    
    if (pointIndex < points.size() && point.getY() > points[pointIndex].getY()) {
        points.insert(points.begin() + pointIndex, point);
    }
}

void RedMonster::killRedPointByIndex(unsigned int index) {
    points[index].kill(time);
    previousPoints.push(points[index]);
    points.erase(points.begin() + index);
}

void RedMonster::cleanLocation(const GlassLocation& location) {
    if (points.empty()) return;
    unsigned int X = location.getX();
    unsigned int Y = location.getY();

    unsigned int pointIndex = 0; 
    while (pointIndex < points.size() - 1) {
        const RedPoint& point = points[pointIndex];
        const RedPoint& nextPoint = points[pointIndex + 1];

        if ((point.isAfter(nextPoint) && X > point.getX() && Y <= nextPoint.getY()) 
            || (!point.isAfter(nextPoint) && X <= point.getX() && Y >= point.getY())) {
                RedPoint newPoint = max(point, nextPoint, time);
                killRedPointByIndex(pointIndex);
                killRedPointByIndex(pointIndex);
                points.insert(points.begin() + pointIndex, newPoint);
                continue;
        }
        pointIndex++;
    }
}

std::vector<GlassLocation> RedMonster::getLocationsForItemIndex(unsigned int index) {
    assert(index < instance->getNbItems());
    const GlassItem& item = instance->getItem(index);
    unsigned int w = item.getWidth();
    unsigned int h = item.getHeight();
    int xMin = getXMax() - MAX_XX;
    int xMax = WIDTH_PLATES - w;
    int yMax = HEIGHT_PLATES - h;
    int xMaxRotated = WIDTH_PLATES - h;
    int yMaxRotated = HEIGHT_PLATES - w;
    std::vector<GlassLocation> locations;
    
    int x = 0;
    int y = 0;
    if (points.empty()) {
        if (x <= xMax && y <= yMax)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time + 1), locations);
        if (x <= xMaxRotated && y <= yMaxRotated)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time + 1), locations);
        return locations;
    }

    unsigned int stepWidth = points.size() > 1 ? 0 : points[1].getX();
    unsigned int stepHeight = points.size() > 1 ? points[0].getY() - points[1].getY() : MIN_WASTE_AREA + 1;

    assert(stepHeight > 0);
    x = 0;
    y = points[0].getY();
    assert(y != 0);
    int yRotated = y + (stepWidth >= h || stepHeight >= MIN_WASTE_AREA ? 0: MIN_WASTE_AREA);
    int yNotRotated = y + (stepWidth >= w || stepHeight >= MIN_WASTE_AREA ? 0 : MIN_WASTE_AREA);

    if (x >= xMin && x <= xMax && yNotRotated <= yMax)
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, yNotRotated,
            NOT_ROTATED, instance, time + 1), locations);

    if (x >= xMin && x <= xMaxRotated && yRotated <= yMaxRotated)
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, yRotated,
            ROTATED, instance, time + 1), locations);
   
    unsigned int pointIndex = 0;
    while (pointIndex < points.size() - 1 && x < std::max(xMax, xMaxRotated)) {
        x = points[pointIndex].getX();
        if (x < xMin) { pointIndex++; continue; }
        y = points[pointIndex + 1].getY();
        stepWidth = points[pointIndex + 1].getX() - x;
        stepHeight = points[pointIndex].getY(); - y;
        assert(stepHeight > 0);
        yNotRotated = y + (stepWidth >= w || stepHeight >= MIN_WASTE_AREA ? 0 : MIN_WASTE_AREA);
        yRotated = y + (stepWidth >= h || stepHeight >= MIN_WASTE_AREA ? 0: MIN_WASTE_AREA);

        assert(x != 0);
        assert(y != 0);
        if (x <= xMax && yNotRotated <= yMax)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, yNotRotated, NOT_ROTATED, instance, time + 1), locations);
        if (x <= xMaxRotated && yRotated <= yMaxRotated)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, yRotated, ROTATED, instance, time + 1), locations);
        pointIndex++;
    }

    x = points.back().getX();
    y = 0;
    if (x <= xMax && y <= yMax)   
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time + 1), locations);
    if (x <= xMaxRotated && y <= yMaxRotated)
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time + 1), locations);

    return locations;
}

bool RedMonster::isFeasibleLocation(const GlassLocation& location) {
    if (location.getYH() > HEIGHT_PLATES)
    assert(location.getXW() <= WIDTH_PLATES);
    assert(location.getYH() <= HEIGHT_PLATES);
    if (location.getXW() < WIDTH_PLATES && WIDTH_PLATES - location.getXW() < MIN_WASTE_AREA)
        return false;
    if (location.getYH() < HEIGHT_PLATES && HEIGHT_PLATES - location.getYH() < MIN_WASTE_AREA) 
        return false;
    if (getXMax() > location.getX() + MAX_XX)
        return false;
    return true;
}

bool RedMonster::isFreeOutOfDefects(const GlassLocation& location) {
    return getPlate().rectangleIsFreeOutOfDefects(location.getX(), location.getY(), location.getXW(), location.getYH());
}

void RedMonster::addLocationsFreeOfDefectsForLocation(const GlassLocation& location, std::vector<GlassLocation>& locations) {
    if (!isFeasibleLocation(location)) return;

    if (isFreeOutOfDefects(location)) {
        locations.push_back(location); 
        return;
    }

    unsigned int width = location.getWidth();
    unsigned int height = location.getHeight();
    unsigned int currentX = location.getX();

    unsigned int yMin = getPlate().getBestY(currentX, location.getY(), width, height);
    //std::cout << currentX << " --> " << yMin << std::endl;
    if (yMin + height < HEIGHT_PLATES) {
        if (yMin + height + MIN_WASTE_AREA < HEIGHT_PLATES || yMin + height == HEIGHT_PLATES){
            GlassLocation newLocation(currentX, yMin, location);
            assert(isFreeOutOfDefects(newLocation));
            locations.push_back(newLocation);
        } else {
            GlassLocation newLocation(currentX, HEIGHT_PLATES - height, location);
            if (isFreeOutOfDefects(newLocation)) locations.push_back(newLocation);
        }
    }

    // ça ne sert à rien d'aller plus bas
    if (yMin == location.getY()) return;

    unsigned int yPrev = yMin;

    const std::vector<GlassDefect>& defects = getPlate().getDefects();
    auto defectIt = defects.begin();
    // Initialiser avec x % location.get_x() // TODO enlever avec les location.getX()
    while (defectIt != defects.end()) {
        currentX = defectIt->getXWidth();
        if (currentX != location.getX()) 
            currentX = std::max(currentX, location.getX() + MIN_WASTE_AREA);

        if (currentX + width != WIDTH_PLATES && currentX + width + MIN_WASTE_AREA > WIDTH_PLATES) { defectIt++; continue; }

        yMin = getPlate().getBestY(currentX, location.getY(), width, height);

        if (yMin > yPrev) break;
        if (yMin < HEIGHT_PLATES && yMin == yPrev) break;

        if (yMin != location.getY()) yMin = std::max(yMin, location.getY() + MIN_WASTE_AREA);   
        if (yMin + height < HEIGHT_PLATES) {
            if (yMin + height + MIN_WASTE_AREA || yMin + height == HEIGHT_PLATES){
                GlassLocation newLocation(currentX, yMin, location);
                assert(isFreeOutOfDefects(newLocation));
                locations.push_back(newLocation);
            } else {
                GlassLocation newLocation(currentX, HEIGHT_PLATES - height, location);
                if (isFreeOutOfDefects(newLocation)) locations.push_back(newLocation);
            }
    }   
        //std::cout << location << std::endl;
        yPrev = yMin;
        defectIt++;
    }

    // TODO remove
    /*for (Location location:locations){
        if (!is_free_out_of_defects(location)){
            //std::cout << location << std::endl;
        }
        assert(is_free_out_of_defects(location));
    }*/
    return;
}

void RedMonster::reviveRedPoint(RedPoint& point) {
    unsigned int index = 0;
    while (index < points.size() && points[index] < point) index++;
    point.revive();
    points.insert(points.begin() + index, point);
}

void RedMonster::revert() {
    time--;
    while(!previousPoints.empty() && previousPoints.top().getDeathTime() > time) {
        RedPoint point = previousPoints.top();
        previousPoints.pop();
        reviveRedPoint(point);
    }

    unsigned int index = 0;
    while (index < points.size()) {
        if (points[index].getBirthTime() <= time) {
            index++;
            continue;
        }
        points.erase(points.begin() + index);
    }
}

unsigned int RedMonster::getXMax() const {
    if (points.empty()) 
        return 0;
    return points.back().getX();
}

// TODO internaliser ? 
unsigned int RedMonster::computeArea() const {
    if (points.empty()) return 0;

    unsigned int xMax = getXMax();
    unsigned int area = 0;
    unsigned int x = xMax > MAX_XX ? xMax - MAX_XX : 0;
    area += x*HEIGHT_PLATES;

    unsigned int pointIndex = 0;    
    while (pointIndex < points.size()){
        if (points[pointIndex].getX() < x) {pointIndex++; continue;}
        area += (points[pointIndex].getX() - x)*points[pointIndex].getY();
        x = points[pointIndex].getX();
        pointIndex++;
    }
    
    return area;
}

std::ostream& operator<<(std::ostream& os, const RedMonster& monster){
    os << "RedMonster #" << monster.getPlateIndex() << " -- ";
    os << monster.getPoints().size() << "pt(s) ";
   
   for (const RedPoint& point: monster.getPoints()){
        std::cout << "\t" << point;
    }
    return os;
}
