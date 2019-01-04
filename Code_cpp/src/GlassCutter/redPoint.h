#pragma once

#include <iostream>

class RedPoint {

    public:
    RedPoint(unsigned int x, unsigned int y, unsigned int itemSequence)
        :x(x), y(y), itemSequence(itemSequence), timeBirth(itemSequence), timeDeath(-1) {

    }

    bool isAfter(const RedPoint& other) const;
    void eat(const RedPoint& casualty);
    void kill();
    void revive();

    bool operator<(const RedPoint& other) const;
    bool operator==(const RedPoint& other) const;
    bool operator!=(const RedPoint& other) const;

    unsigned int getX() const { return x; }
    unsigned int getY() const { return y; }
    unsigned int getItemSequence() const { return itemSequence; }
    unsigned int getTimeBirth() const { return timeBirth; }
    unsigned int getTimeDeath() const { return timeDeath; }

    private:
    unsigned int x;
    unsigned int y;
    unsigned int itemSequence;
    unsigned int timeBirth;
    unsigned int timeDeath;
};

RedPoint max(RedPoint pointA, RedPoint pointB, unsigned int birth);
std::ostream& operator<<(std::ostream& os, const RedPoint& point);