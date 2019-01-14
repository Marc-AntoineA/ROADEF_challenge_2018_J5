#pragma once

#include <sstream>

class GlassItem {

    public:    
    GlassItem(std::string input_line);
    
    unsigned int getHeight() const;
    unsigned int getHeight(bool rotated) const;
    unsigned int getWidth() const;
    unsigned int getWidth(bool rotated) const;
    unsigned int getId() const;
    unsigned int getStackId() const;
    unsigned int getArea() const;
    unsigned int getStackSequence() const;

    private:
    unsigned int width;
    unsigned int height;
    unsigned int id;
    unsigned int stackId;
    unsigned int stackSequence;
    
};

std::ostream& operator<<(std::ostream& os, const GlassItem item);