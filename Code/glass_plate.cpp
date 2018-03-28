#include "glass_plate.h"

using namespace std;

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

std::ostream& operator<<(std::ostream& os, const GlassPlate& plate){
    os << "Id " << plate.Getplate_id() << "\t H " << plate.Getheight() << "\t W " << plate.Getwidth() << endl;
    for(int k = 0; k < plate.Getdefect_nbr(); k++){
        os << "\t" << plate.Getdefect(k) << endl;
    }
    return os;
}