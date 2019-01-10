#pragma once

#include <string>
#include <sstream>

class GlassDefect {

    public:
    GlassDefect(int x, int y, int width, int height, int id, int plateId):
         x(x), y(y), width(width), height(height), id(id), plateId(plateId) {} 

    GlassDefect(std::string input_line);
    unsigned int getId() const { return id; } 
    unsigned int getX() const { return x; }
    unsigned int getY() const { return y; }
    unsigned int getHeight() const { return height; }
    unsigned int getWidth() const { return width; }
    unsigned int getXWidth() const { return x + width; }
    unsigned int getYHeight() const { return y + height; }
    unsigned int getPlateId() const { return plateId; }
    bool intersectLine(unsigned int otherX, unsigned int otherY, unsigned int otherHeight, bool vertical) const;

    private:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int id;
    unsigned int plateId;
};

std::ostream& operator<<(std::ostream& os, const GlassDefect defect);
bool operator<(const GlassDefect &defect1, const GlassDefect &defect2);

struct {
    bool operator()(const GlassDefect &defect1, const GlassDefect &defect2) const
    {   
        return defect1.getY() < defect2.getY();
    }   
} sortY;