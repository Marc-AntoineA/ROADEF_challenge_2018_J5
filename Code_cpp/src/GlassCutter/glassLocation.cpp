#include "glassLocation.h"

std::ostream& operator<<(std::ostream& os, const GlassLocation& location) {
    os << location.getItemIndex() << " : " << "(" << location.getX() << ", " << location.getY() << ", ";
    os << location.getXW() << ", " << location.getYH() << ")";
    os << " -sequence " << location.getLocationSequence();
    os << " -bin " << location.getBinId(); 
    return os;
}
