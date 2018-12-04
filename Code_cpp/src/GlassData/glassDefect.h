#pragma once

#include <string>
#include <sstream>

class GlassDefect {

    public:
    GlassDefect(int x, int y, int width, int height, int id, int plateId):
         x(x), y(y), width(width), height(height), id(id), plateId(plateId) {} 

    GlassDefect(std::string input_line);
    int getId() const { return id; } 
    int getX() const { return x; }
    int getY() const { return y; }
    int getHeight() const { return height; }
    int getWidth() const { return width; }
    int getXWidth() const { return x + width; }
    int getYHeight() const { return y + height; }
    int getPlateId() const { return plateId; }
    bool intersectLine(int otherX, int otherY, int otherHeight, bool vertical) const;

    private:
    int x;
    int y;
    int width;
    int height;
    int id;
    int plateId;
};

std::ostream& operator<<(std::ostream& os, const GlassDefect defect);
bool operator<(const GlassDefect &defect1, const GlassDefect &defect2);

struct {
    bool operator()(const GlassDefect &defect1, const GlassDefect &defect2) const
    {   
        return defect1.getY() < defect2.getY();
    }   
} sortY;