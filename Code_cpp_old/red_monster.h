#pragma once

#include "red_point.h"
#include "location.h"
#include "instance.h"
#include "glass_plate.h"

#include <vector>
#include <queue>
#include <sstream>
#include <iostream> // TODO remove

class RedMonster {


    public:
    RedMonster();
    RedMonster(GlassPlate* pl);
    void reset();
    int get_area();
    int get_margin();
    void incr_red_monster(const Location& location);
    std::vector<Location> get_locations(GlassItem* item);
    void rollback();
    GlassPlate* get_plate() { return plate; }
    std::vector<RedPoint>* get_points() { return &points; }
    int get_nb_items() { return nb_items; }
    bool operator==(RedMonster& other);
    int get_x_max () const;
    
    private:
    void incr_green_star(RedPoint new_point);
    void clean(int x, int y);
    void RedPoint_resurrection(RedPoint death_point);
    void RedPoint_kill(int k);
    void add_locations_free_of_defects(std::vector<Location>& locations, const Location& location);
    bool check_location(const Location& location);

    std::vector<RedPoint> points;
    std::priority_queue<RedPoint, std::vector<RedPoint>, sortSequenceClass> old_points;
    int nb_items;
    GlassPlate* plate;
};

std::ostream& operator<<(std::ostream& os, RedMonster& monster);