#include "redMonster.h"

#include "../GlassData/glassConstants.h"


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
            || (!point.isAfter(nextPoint) && X <= point.getX() && Y >= nextPoint.getY())) {
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
    unsigned int xMax = WIDTH_PLATES - w;
    unsigned int yMax = HEIGHT_PLATES - h;
    unsigned int xMaxRotated = WIDTH_PLATES - h;
    unsigned int yMaxRotated = HEIGHT_PLATES - w;
    std::vector<GlassLocation> locations;
    if (points.empty()) {
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, 0, 0, ROTATED, instance, time), locations);
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, 0, 0, NOT_ROTATED, instance, time), locations);
        return locations;
    }

    unsigned int x = 0;
    unsigned int y = points[0].getY();
    if (y <= yMax) 
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time), locations);
    if (y <= yMaxRotated)
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time), locations);
   
    unsigned int pointIndex = 0;
    while (pointIndex < points.size() - 1 && x < std::max(xMax, xMaxRotated)) {
        x = points[pointIndex].getX();
        y = points[pointIndex + 1].getY();
        if (y <= yMax)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time), locations);
        if (y <= yMaxRotated)
            addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time), locations);
        
        pointIndex++;
    }

    if (x <= xMax)  
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, NOT_ROTATED, instance, time), locations);
    if (x <= xMaxRotated)
        addLocationsFreeOfDefectsForLocation(GlassLocation(index, plateIndex, x, y, ROTATED, instance, time), locations);
    
    return locations;
}

// todo
bool RedMonster::isFeasibleLocation(const GlassLocation& location) {
    return true;
}

// TODO gérer les 'free of defects'
void RedMonster::addLocationsFreeOfDefectsForLocation(const GlassLocation& location, std::vector<GlassLocation>& locations) {
    if (!isFeasibleLocation(location)) return;
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