#include "GlassStructures.h"

/////GlassDefect/////

GlassDefect::GlassDefect()
{
    //ctor
}

GlassDefect::GlassDefect(unsigned int id, unsigned int pid, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    defect_id = id;
    plate_id = pid;
    pos_x = x;
    pos_y = y;
    width = w;
    height = h;
}

GlassDefect::~GlassDefect()
{
    //dtor
}

/////GlassItem/////

GlassItem::GlassItem()
{
    //ctor
}

GlassItem::~GlassItem()
{
    //dtor
}

/////GlassNode/////

GlassNode::GlassNode()
{
    plate_id = 0;
    node_id = 0;
    pos_x = 0;
    pos_y = 0;
    width = 0;
    height = 0;
    type = 0;
    cut = 0;
    parent = 0;
    successor_nbr = 0;
}

GlassNode::~GlassNode()
{
    //dtor
}

/////GlassPlate/////

GlassPlate::GlassPlate()
{
    //ctor
    waste = 0.0;
    useful = 0.0;
    successor_nbr = 0;
    defect_nbr = 0;
    residual.Setwidth(0);
    residual.Setheight(0);
}

GlassPlate::~GlassPlate()
{
    //dtor
}

/////GlassStack/////

GlassStack::GlassStack()
{
    //ctor
    cur_item_idx = 0;
    item_nbr = 0;
    idx = 0;
}

GlassStack::~GlassStack()
{
    //dtor
}
