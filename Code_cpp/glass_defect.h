#pragma once

#include <string>
#include <sstream>

class GlassDefect {

    public:
    GlassDefect(int x, int y, int w, int h, int id, int plate_id):
         m_x(x), m_y(y), m_w(w), m_h(h), m_id(id), m_plate_id(plate_id) {} 

    GlassDefect(std::string input_line);
    int get_id() const { return m_id; } 
    int get_x() const { return m_x; }
    int get_y() const { return m_y; }
    int get_h() const { return m_h; }
    int get_w() const { return m_w; }
    int get_xw() const { return m_x + m_w; }
    int get_yh() const { return m_y + m_h; }
    int get_plate_id() const { return m_plate_id; }
    bool intersect_line(int x, int y, int w, bool vertical) const;

    private:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    int m_id;
    int m_plate_id;
};

std::ostream& operator<<(std::ostream& os, const GlassDefect defect);
bool operator<(const GlassDefect &defect1, const GlassDefect &defect2);

struct {
    bool operator()(const GlassDefect &defect1, const GlassDefect &defect2) const
    {   
        return defect1.get_y() < defect2.get_y();
    }   
} sortY;