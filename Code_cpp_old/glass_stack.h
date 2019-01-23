#pragma once

#include "glass_item.h"

#include <sstream>
#include <vector>

// TODO enlever le pointeur vers le tableau mais 
// passer mettre un pointeur sur l'objet instance
class GlassStack {

    public:
    GlassStack();
    GlassItem* top();
    GlassItem* pop();
    void push();
    void push(GlassItem* item);
    bool empty();
    void reset();
    
    private:
    int m_size;
    int current_pos;
    std::vector<GlassItem*> items;
};

std::ostream& operator<<(std::ostream& os, GlassStack glassStack);