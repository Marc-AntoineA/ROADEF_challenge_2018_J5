#include "glass_item.h"

using namespace std;

/////GlassItem/////

GlassItem::GlassItem()
{
    //ctor
}

GlassItem::~GlassItem()
{
    //dtor
}

ostream& operator<<(ostream& os, const GlassItem& item){
    os << "Id " << item.Getitem_id() << "\tStk " << item.Getitem_stack() 
        <<"\tSeq " << item.Getitem_seq() << "\tw " << item.Getitem_w()
        <<"\th " << item.Getitem_h();
    return os;
}