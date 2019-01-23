#include "glass_item.h"

#include <string>
#include <sstream>
#include <iostream>

GlassItem::GlassItem(std::string input_line){

    std::stringstream line(input_line);
    std::string entity;

    std::getline(line, entity, ';');
    m_id = std::stoi(entity);

    std::getline(line, entity, ';');
    m_h = std::stoi(entity);

    std::getline(line, entity, ';');
    m_w = std::stoi(entity);

    std::getline(line, entity, ';');
    m_id_stack = std::stoi(entity);

    std::getline(line, entity, ';');
    m_stack_sequence = std::stoi(entity);
}

unsigned int GlassItem::get_h() const {
    return m_h;
}

unsigned int GlassItem::get_h(bool rotated) const {
    return rotated ? m_w: m_h;
}

unsigned int GlassItem::get_w() const {
    return m_w;
}

unsigned int GlassItem::get_w(bool rotated) const {
    return rotated ? m_h: m_w;
}

unsigned int GlassItem::get_area() const {
    return m_w*m_h;
}

unsigned int GlassItem::get_id() const {
    return m_id;
}

unsigned int GlassItem::get_id_stack() const{
    return m_id_stack;
}

unsigned int GlassItem::get_stack_sequence() const{
    return m_id_stack;
}

std::ostream& operator<<(std::ostream& os, GlassItem item){
    os << item.get_id() << " : (" << item.get_w() << ", " 
        << item.get_h() << ") : " << item.get_id_stack() << " -- " << item.get_stack_sequence() << std::endl; 

    return os;
}