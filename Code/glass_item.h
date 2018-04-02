#pragma once
#include <iostream>

/////GlassItem/////

/* This class is created
 * to model a batch item instance with its all attributes
**/
class GlassItem
{
    public:
        GlassItem();
        virtual ~GlassItem();
        int Getitem_id() const { return item_id; }
        int Getitem_w(bool rotated = false) const { if(rotated){return item_h;} else {return item_w;} }
        int Getitem_h(bool rotated = false) const { if(rotated){return item_w;} else {return item_h;} }
        int Getitem_stack() const { return item_stack; }
        int Getitem_seq() const { return item_seq; }

        void Setitem_id(int val) { item_id = val; }
        void Setitem_w(int val) { item_w = val; }
        void Setitem_h(int val) { item_h = val; }
        void Setitem_stack(int val) { item_stack = val; }
        void Setitem_seq(int val) { item_seq = val; }

    protected:
    private:
        int item_id; // Item's Id.
        int item_w; // Item's width.
        int item_h; // Item's height.
        int item_stack; // Item's stack Id
        int item_seq; // Item's stack sequence.
};

std::ostream& operator<<(std::ostream& os, const GlassItem& item);