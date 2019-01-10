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
    const GlassItem& item = instance->getItem(index);
    unsigned int w = item.getWidth();
    unsigned int h = item.getHeight();
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

    x = 0;
    y = points[0].getY();
    assert(y != 0);
    if (x <= xMax && y <= yMax) 
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time + 1), locations);
    if (x <= xMaxRotated && y <= yMaxRotated)
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time + 1), locations);
   
    unsigned int pointIndex = 0;
    while (pointIndex < points.size() - 1 && x < std::max(xMax, xMaxRotated)) {
        x = points[pointIndex].getX();
        y = points[pointIndex + 1].getY();
        assert(x != 0);
        assert(y != 0);
        if (x <= xMax && y <= yMax)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time + 1), locations);
        if (x <= xMaxRotated && y <= yMaxRotated)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time + 1), locations);
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

// todo
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


// TODO gérer les 'free of defects'
void RedMonster::addLocationsFreeOfDefectsForLocation(const GlassLocation& location, std::vector<GlassLocation>& locations) {
    if (!isFeasibleLocation(location)) return;
    assert(getPlate().rectangleIsFreeOutOfDefects(location.getX(), location.getY(), location.getXW(), location.getYH()));
    locations.push_back(location);
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

unsigned int RedMonster::getXMax() {
    if (points.empty()) 
        return 0;
    return points.back().getX();
}

std::ostream& operator<<(std::ostream& os, const RedMonster& monster){
    os << "RedMonster #" << monster.getPlateIndex() << " -- ";
    os << monster.getPoints().size() << "pt(s) ";
   
   for (const RedPoint& point: monster.getPoints()){
        std::cout << "\t" << point;
    }
    return os;
}
