#pragma once

#include "location.h"
#include "glass_cut.h"
#include "glass_plate.h"

#include <fstream>
#include <sstream>

const int BRANCH = -2;
const int WASTE = -1;
const int RESIDUAL = -3;

struct RealCut {
    unsigned int x;
    unsigned int nb_items;

    RealCut(unsigned int t_x, unsigned int t_nb_items): x(t_x), nb_items(t_nb_items) {};
};

class GlassNode {
    public:
    GlassNode(){}
    GlassNode(GlassPlate* plate); // Node constructor
    GlassNode(int x, int y, int w, int h, int type, int depth, GlassPlate* plate)
        : m_x(x), m_y(y), m_w(w), m_h(h), m_type(type), m_depth(depth), m_plate(plate) 
        { sons = std::vector<GlassNode>();}
    
    // renvoie le nombre d'items découpés
    int build_node(std::vector<Location>::iterator first, std::vector<Location>::iterator last);
    int check_node(std::vector<Location>::iterator first, std::vector<Location>::iterator last);
    int save_node(std::ofstream& output_file, int node_id, int parent_id, int bin_id, bool last);
    
    void set_plate(GlassPlate* plate);
    int get_x() const { return m_x; }
    int get_y() const { return m_y; }
    int get_w() const { return m_w; }
    int get_h() const { return m_h; }
    int get_xw() const { return m_x + m_w; }
    int get_yh() const { return m_y + m_h; }
    int get_type() const { return m_type; }
    void set_type(int type) { m_type  = type; }
    int get_depth() const { return m_depth; }
    std::vector<GlassNode>* get_sons() { return &sons; }

    private:
    bool is_free_of_defects(const GlassCut& cut);

    int m_x;
    int m_y;
    int m_w;
    int m_h;
    int m_type;
    int m_depth;
    GlassPlate* m_plate;
    std::vector<GlassNode> sons;
};

std::ostream& operator<<(std::ostream& os, GlassNode& node);