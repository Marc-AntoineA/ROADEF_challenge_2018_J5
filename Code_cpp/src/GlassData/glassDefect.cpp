#include "glassDefect.h"
#include <iostream>

GlassDefect::GlassDefect(std::string inputLine){

    // Defect id, plate id, x, y, w, h
    std::stringstream line(inputLine);
    std::string entity;

    std::getline(line, entity, ';');
    id = std::stoi(entity);

    std::getline(line, entity, ';');
    plateId = std::stoi(entity);

    std::getline(line, entity, ';');
    x = std::stoi(entity);

    std::getline(line, entity, ';');
    y = std::stoi(entity);

    std::getline(line, entity, ';');
    width = std::stoi(entity);

    std::getline(line, entity, ';');
    height = std::stoi(entity);

}

// TODO checker mais ça semble correct
bool GlassDefect::intersectLine(unsigned int otherX, unsigned int otherY, unsigned int otherHeight, bool vertical) const {
    if (vertical){
        if (x <= otherX && x + width > otherX && y >= otherY && y <= otherY + otherHeight)
            return true;
        if (x <= otherX && x + width > otherX && y + height >= otherY && y + height <= otherY + otherHeight)
            return true;
        return false;
    } else {
        if (y <= otherX && y + height > otherX && x >= otherY && x <= otherY + otherHeight)
            return true;
        if (y <= otherX && y + height > otherX && x + width >= otherY && x + width <= otherY + otherHeight)
            return true;
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, GlassDefect defect){
    os << "D : " << defect.getX() << " " << defect.getY() << " " << defect.getXWidth();
    os << " " << defect.getYHeight() << std::endl;
    return os;
}

bool operator<(const GlassDefect &defect1, const GlassDefect &defect2){
    return defect1.getX() < defect2.getX();
}