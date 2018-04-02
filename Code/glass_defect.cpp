#include "glass_defect.h"


/////GlassDefect/////

GlassDefect::GlassDefect()
{
    //ctor
}

GlassDefect::GlassDefect(int id, int pid, int x, int y, int w, int h)
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

std::ostream& operator<<(std::ostream& os, const GlassDefect& defect){
    os << "Id " << defect.Getdefect_id();
    os << "\tPlate " << defect.Getplate_id();
    os << "\tW " << defect.Getwidth();
    os << "\tH " << defect.Getheight();
    os << "\tx " << defect.Getpos_x();
    os << "\ty " << defect.Getpos_y();
    return os;
}