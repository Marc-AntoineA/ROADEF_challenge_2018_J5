#include "location.h"

// Score à minimiser
int Location::compute_score() const {
    int w_x = 50000;
    int w_y = 4000; 
    int margin_x = 50000;
    int margin_y = 50000;

    return  w_x*get_xw()
        - w_y*get_yh()
        - margin_x*std::max(0, get_yh() - 2900)
        - margin_y*std::max(0, get_xw() - 5900)
        - 10*m_item->get_area();
}

bool Location::operator<(const Location& loc) const {
    if (m_bin_id == loc.get_bin_id())
        return get_score() < loc.get_score();
    return m_bin_id < loc.get_bin_id();
}

std::ostream& operator<<(std::ostream& os, const Location& location) {
    os << location.get_item()->get_id() << " : " << "(" << location.get_x() << ", " << location.get_y() << ", ";
    os << location.get_xw() << ", " << location.get_yh() << ")";
    os << " - " << location.get_item_pos();
    os << " -b " << location.get_bin_id(); 
    return os;
}
