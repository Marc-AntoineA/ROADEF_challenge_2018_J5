#include "glassLocation.h"

GlassLocation::GlassLocation(unsigned int x, unsigned int y, const GlassLocation& location){
        this->itemIndex = location.getItemIndex();
        this->rotated = location.getRotated();
        this->binId = location.getBinId();
        this->x = x;
        this->y = y;
        this->locationSequence = location.getLocationSequence();
        this->instance = location.getInstance();
}

std::ostream& operator<<(std::ostream& os, const GlassLocation& location) {
    os << location.getItemIndex() << " : " << "(" << location.getX() << ", " << location.getY() << ", ";
    os << location.getXW() << ", " << location.getYH() << ")";
    os << " -sequence " << location.getLocationSequence();
    os << " -bin " << location.getBinId(); 
    return os;
}
