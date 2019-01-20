#pragma once

#include <iostream>
#include <limits>

class RedPoint {

    public:
    RedPoint(unsigned int x, unsigned int y, unsigned int time)
        :x(x), y(y), sequencePosition(time), birthTime(time), deathTime(0) {}
    RedPoint(unsigned int birth)
        : x(0), y(0), sequencePosition(0), birthTime(birth), deathTime(0) {}
    RedPoint() : x(0), y(0), sequencePosition(0), birthTime(0), deathTime(0) {}
    
    bool isAfter(const RedPoint& other) const;
    void eat(const RedPoint& casualty);
    void kill(unsigned int time);
    void revive();

    bool operator<(const RedPoint& other) const;
    bool operator==(const RedPoint& other) const;
    bool operator!=(const RedPoint& other) const;

    unsigned int getX() const { return x; }
    void setX(unsigned int x) { this->x = x; }
    unsigned int getY() const { return y; }
    void setY(unsigned int y) { this->y = y; }
    unsigned int getSequencePosition() const { return sequencePosition; }
    void setSequencePosition(unsigned int sequencePosition) { this->sequencePosition = sequencePosition; }
    unsigned int getBirthTime() const { return birthTime; }
    void setBirthTime(unsigned int birthTime) { this->birthTime = birthTime; }
    unsigned int getDeathTime() const { return deathTime; }
    void setDeathTime(unsigned int deathTime) { this->deathTime = deathTime; }

    private:
    unsigned int x;
    unsigned int y;
    unsigned int sequencePosition;
    unsigned int birthTime;
    unsigned int deathTime;
};

RedPoint max(RedPoint pointA, RedPoint pointB, unsigned int birth);
std::ostream& operator<<(std::ostream& os, const RedPoint& point);

struct sortRedPointsByDeathTime {
    bool operator() (RedPoint& a, RedPoint& b) {
        return (a.getDeathTime() < b.getDeathTime());
    }
};