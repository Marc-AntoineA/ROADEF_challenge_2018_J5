#include "red_point.h"

#include <algorithm>

RedPoint::RedPoint(int b): birth(b) {
    death = -1;
}

bool RedPoint::operator< (const RedPoint &rp) const {
    if (m_x == rp.m_x)
        return m_y < rp.m_y;
    return m_x < rp.m_x;
}

bool RedPoint::is_after(const RedPoint &rp) const {
    return m_item_sequence > rp.m_item_sequence;
}

// TODO : gérer la sauvegarde des états précédents pour le rollback ?
void RedPoint::eat (const RedPoint &casualty) {
    m_item_sequence = std::max(m_item_sequence, casualty.m_item_sequence);
    m_x = std::max(m_x, casualty.m_x);
    m_y = std::max(m_y, casualty.m_y);
}

RedPoint max(RedPoint ptA, RedPoint ptB, int b){
    RedPoint sum(b);
    sum.set_x(std::max(ptA.get_x(), ptB.get_x()));
    sum.set_y(std::max(ptA.get_y(), ptB.get_y()));
    sum.set_item_sequence(std::max(ptA.get_item_sequence(), ptB.get_item_sequence()));
    return sum;
}

std::ostream& operator<<(std::ostream& os, const RedPoint& point){
    os << point.get_birth() << " --> " << point.get_death() << " : ";
    os <<  point.get_item_sequence() << " - " <<"(" << point.get_x() << ", " << point.get_y();
    os << ")" << std::endl;
}