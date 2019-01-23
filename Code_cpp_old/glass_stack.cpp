#include "glass_stack.h"

#include <iostream>

// TODO gérer les sorties de position

GlassStack::GlassStack() {
    items = std::vector<GlassItem*>();
    current_pos = 0;
}

void GlassStack::push() {
    current_pos--;
    if (current_pos >= items.size()) current_pos = items.size() - 1;
}

void GlassStack::push(GlassItem* item){
    items.push_back(item);
}

void GlassStack::reset(){
    current_pos = 0;
}

GlassItem* GlassStack::top() {
    return items[current_pos];
}

GlassItem* GlassStack::pop() {
    if (current_pos >= items.size() ) throw "stack empty";
    GlassItem* item = items[current_pos];
    current_pos++;
    return item;
}

bool GlassStack::empty() {
    return current_pos == items.size();
}
// TODO
std::ostream& operator<<(std::ostream& os, GlassStack glassStack) {
    return os << glassStack.top()->get_id() << std::endl;
}