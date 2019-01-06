#include "redMonster.h"

RedMonster::RedMonster() {
    
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
