#include "glass_defect.h"
#include <iostream>

GlassDefect::GlassDefect(std::string input_line){

    // Defect id, plate id, x, y, w, h
    std::stringstream line(input_line);
    std::string entity;

    std::getline(line, entity, ';');
    m_id = std::stoi(entity);

    std::getline(line, entity, ';');
    m_plate_id = std::stoi(entity);

    std::getline(line, entity, ';');
    m_x = std::stoi(entity);

    std::getline(line, entity, ';');
    m_y = std::stoi(entity);

    std::getline(line, entity, ';');
    m_w = std::stoi(entity);

    std::getline(line, entity, ';');
    m_h = std::stoi(entity);

}

// TODO checker mais ça semble correct
bool GlassDefect::intersect_line(int x, int y,  int h, bool vertical) const {
    if (vertical){
        if (m_x <= x && m_x + m_w > x && m_y >= y && m_y <= y + h)
            return true;
        if (m_x <= x && m_x + m_w > x && m_y + m_h >= y && m_y + h <= y + h)
            return true;
        return false;
    } else {
        if (m_y <= x && m_y + m_h > x && m_x >= y && m_x <= y + h)
            return true;
        if (m_y <= x && m_y + m_h > x && m_x + m_w >= y && m_x + m_w <= y + h)
            return true;
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, GlassDefect defect){
    os << "D : " << defect.get_x() << " " << defect.get_y() << " " << defect.get_xw() << " " << defect.get_yh() << std::endl;
}

bool operator<(const GlassDefect &defect1, const GlassDefect &defect2){
    return defect1.get_x() < defect2.get_x();
}