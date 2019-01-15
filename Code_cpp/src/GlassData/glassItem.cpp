#include "glassItem.h"

#include <string>
#include <sstream>
#include <iostream>
#include <cassert>

GlassItem::GlassItem(std::string input_line){

    std::stringstream line(input_line);
    std::string entity;

    std::getline(line, entity, ';');
    id = std::stoi(entity);

    std::getline(line, entity, ';');
    height = std::stoi(entity);

    std::getline(line, entity, ';');
    width = std::stoi(entity);

    std::getline(line, entity, ';');
    stackId = std::stoi(entity);

    std::getline(line, entity, ';');
    stackSequence = std::stoi(entity);
}

unsigned int GlassItem::getHeight() const {
    return height;
}

unsigned int GlassItem::getHeight(bool rotated) const {
    return rotated ? width : height;
}

unsigned int GlassItem::getWidth() const {
    return width;
}

unsigned int GlassItem::getWidth(bool rotated) const {
    return rotated ? height : width;
}

unsigned int GlassItem::getArea() const {
    return width*height;
}

unsigned int GlassItem::getId() const {
    return id;
}

unsigned int GlassItem::getStackId() const{
    return stackId;
}

unsigned int GlassItem::getStackSequence() const {
    return stackSequence;
}

std::ostream& operator<<(std::ostream& os, GlassItem item){
    os << item.getId() << " : (" << item.getWidth() << ", " 
        << item.getHeight() << ") : " << item.getStackId() << " -- " << item.getStackSequence() << std::endl; 

    return os;
}