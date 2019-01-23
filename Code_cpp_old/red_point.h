#pragma once

#include <sstream>

class RedPoint {

    public:
    RedPoint(int x, int y, int item_sequence):
        m_x(x), m_y(y), m_item_sequence(item_sequence){ death = -1; birth = item_sequence;}
    RedPoint(int b);
    bool operator<(const RedPoint &rp) const;
    bool operator==(const RedPoint &other){
        return m_x == other.get_x() && m_y == other.get_y() 
            && m_item_sequence == other.get_item_sequence() 
            && death == other.get_death()
            && birth == other.get_birth();
    }
    bool operator!=(const RedPoint &other){
        return !(*this == other);
    }
    bool is_after(const RedPoint &rp) const;
    void eat(const RedPoint &casualty);
    int get_x() const { return m_x; }
    int get_y() const { return m_y; }
    int get_item_sequence() const { return m_item_sequence; }
    int get_death() const { return death; }
    int get_birth() const { return birth; }
    void set_x(int x) { m_x = x; }
    void set_y(int y) { m_y = y; }
    void set_item_sequence(int item_sequence) { m_item_sequence = item_sequence; }
    void kill(int d) { death = d; }
    void rebirth() { death = -1; }

    private:
    int m_x;
    int m_y;
    int m_item_sequence;
    int birth;
    int death;
};

RedPoint max(RedPoint ptA, RedPoint ptB, int b);
std::ostream& operator<<(std::ostream& os, const RedPoint& point);

struct sortSequenceClass {
  bool operator() (RedPoint a, RedPoint b) { return (a.get_death() < b.get_death());}
};