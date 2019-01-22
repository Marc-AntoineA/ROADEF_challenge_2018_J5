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

bool GlassLocation::operator==(const GlassLocation& otherLocation) const {
    return itemIndex == otherLocation.getItemIndex() && rotated == otherLocation.getRotated() 
        && x == otherLocation.getX() && y == otherLocation.getY() 
        && binId == otherLocation.getBinId() && locationSequence == otherLocation.getLocationSequence();       
}

bool GlassLocation::operator<(const GlassLocation& otherLocation) const {
    if (itemIndex == otherLocation.getItemIndex()) {
        if (rotated == otherLocation.getRotated()) {
            if (x == otherLocation.getX()) {
                if (y == otherLocation.getY()) {
                    if (binId == otherLocation.getBinId()) {
                        return locationSequence < otherLocation.getLocationSequence();
                    }
                    return binId < otherLocation.getBinId();
                }
                return y < otherLocation.getY();
            }
            return x < otherLocation.getX();
        }
        return rotated < otherLocation.getRotated();
    }
    return itemIndex < otherLocation.getItemIndex();
}

std::ostream& operator<<(std::ostream& os, const GlassLocation& location) {
    assert(location.getWidth() != 0);
    assert(location.getX() != location.getXW());
    os << location.getItemIndex() << " : " << "(" << location.getX() << ", " << location.getY() << ", ";
    os << location.getXW() << ", " << location.getYH() << ")";
    os << " -sequence " << location.getLocationSequence();
    os << " -bin " << location.getBinId(); 
    return os;
}