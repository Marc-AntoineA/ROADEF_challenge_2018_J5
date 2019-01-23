#pragma once

#include <vector>
#include <string>

#include "glass_item.h"
#include "glass_stack.h"
#include "glass_plate.h"

const int nPlates = 100;
const int widthPlates = 6000;
const int heightPlates = 3210;
const int minXX = 100;
const int maxXX = 3500;
const int minYY = 100;
const int minWaste = 20;
const int nbPlates = 100;

const bool VERBOSE = false;

const std::string PATH = "instances_checker/";
//const std::string PATH = "";

class Instance {

    public:
    Instance(std::string instance_name);
    Instance(){}
    unsigned int get_nb_items() const { return items.size(); }
    GlassItem* get_item(int i) { return &items[i]; }
    std::string get_instance_name() const { return m_instance_name; }
    GlassPlate* get_plate(int i) { return &glass_plates[i]; }
    unsigned int get_area() const { return area; }
    bool check_location();

    private:
    void build_items();
    void build_plates();
    std::vector<GlassItem> items;
    std::string m_instance_name;
    std::vector<GlassPlate> glass_plates;
    unsigned int area;
};
