#pragma once

#include "glass_item.h"
#include <vector>

/////GlassStack/////

/* This class is created
 * to model a batch stack instance with its all attributes
**/
class GlassStack
{
    public:
        GlassStack();
        virtual ~GlassStack();
        int Getstack_id() const { return stack_id; }
        int Getitem_nbr() const { return item_nbr; }
        int Getcur_item_idx() const { return cur_item_idx; }
        GlassItem Getitem(int idx) const { return items[idx]; }

        void Setstack_id(int val){ stack_id = val; }
        void Setitem_nbr(int val){ item_nbr = val; }
        void Setcur_item_idx(int val){ cur_item_idx = val; }
        void Setitem (GlassItem i){ items[idx] = i; idx++; }// rajoute l'item à la fin

        void AllocateItems(){ items.resize(item_nbr); }
        void Increaseitem_nbr(){ item_nbr++; }
        void Pop(){ if(cur_item_idx >= item_nbr) {cur_item_idx = 0xffff;} else { cur_item_idx++; } }
        void Reset(){ cur_item_idx = 0; }
        GlassItem Top(){ return Getitem(cur_item_idx); }
        bool Depleted(){ return cur_item_idx >= item_nbr; }

    public:
        
    
    protected:
    private:
        std::vector<GlassItem> items; // Items list.
        int stack_id; // Stack Id.
        int item_nbr;  // Stack's associated items number.
        int cur_item_idx; // Current items index of stack instance.
        int idx; // Used to add item on top of items list of stack instance.
};
