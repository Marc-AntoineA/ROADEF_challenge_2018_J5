#include "glass_node.h"
#include <sstream>

/////GlassNode/////

using namespace std;

GlassNode::GlassNode()
{
    plate_id = 0;
    node_id = 0;
    pos_x = 0;
    pos_y = 0;
    width = 0;
    height = 0;
    type = UNDEFINED;
    cut = 0;
    parent = UNDEFINED;
    successor_nbr = 0;
}

GlassNode::~GlassNode()
{
    //dtor
}

string GlassNode::toString() const{
    stringstream result;
    result << plate_id << ";"
           << node_id << ";"
           << pos_x << ";"
           << pos_y << ";"
           << width << ";"
           << height << ";"
           << type << ";"
           << cut << ";";
    if (parent != UNDEFINED)
        result << parent;
    return result.str();
}

// à compléter
ostream& operator<<(ostream& os, const GlassNode& node){
    os << "Bin " << node.Getplate_id()
       << "\t X " << node.Getpos_x()
       << "\t Y " << node.Getpos_y()
       << "\t W " << node.Getwidth()
       << "\t H " << node.Getheight()
       << "\t TYPE " << node.Gettype()
       << endl;
    return os;
}
