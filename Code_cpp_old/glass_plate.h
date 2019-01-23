#pragma once

#include <vector>

#include "glass_defect.h"
#include "location.h"

class GlassPlate {

    public:
    GlassPlate();
    GlassPlate(int plate_id);
    void add_defect(GlassDefect defect);
    bool is_free_out_of_defects(const Location&_location) const;
    bool is_free_out_of_defects(int x, int y, int h, bool vertical);
    std::vector<Location> locations_free_out_of_defects(const Location &location);
    int get_plate_id() const;
    void set_plate_id(int plate_id);
    
    private:
    int get_best_y(const Location &location, int x);

    std::vector<GlassDefect> defects;
    
    int m_plate_id;
};