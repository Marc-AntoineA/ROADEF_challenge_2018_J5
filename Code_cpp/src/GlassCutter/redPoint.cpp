#include "redPoint.h"

void RedPoint::kill(unsigned int time) {
    deathTime = time;
}

void RedPoint::revive() {
    deathTime = std::numeric_limits<unsigned int>::max();
}

bool RedPoint::isAfter(const RedPoint& other) const {
    return sequencePosition > other.getSequencePosition();
}

void RedPoint::eat(const RedPoint& casualty) {
    sequencePosition = std::max(sequencePosition, casualty.getSequencePosition());
    x = std::max(x, casualty.getX());
    y = std::max(y, casualty.getY());
}

bool RedPoint::operator==(const RedPoint& other) const {
    return x == other.getX() 
        && y == other.getY()
        && sequencePosition == other.getSequencePosition()
        && deathTime == other.getDeathTime()
        && birthTime == other.getBirthTime();
}

bool RedPoint::operator!=(const RedPoint& other) const {
    return !((*this) == other);
}

bool RedPoint::operator<(const RedPoint& other) const {
    if (x == other.getX())
        return y < other.getY();
    return x < other.getX();
}

RedPoint max(RedPoint pointA, RedPoint pointB, unsigned int birth) {
    RedPoint maxPoint(birth);
    maxPoint.setX(std::max(pointA.getX(), pointB.getX()));
    maxPoint.setY(std::max(pointA.getY(), pointB.getY()));
    maxPoint.setSequencePosition(std::max(pointA.getSequencePosition(), pointB.getSequencePosition()));
    return maxPoint;
}
