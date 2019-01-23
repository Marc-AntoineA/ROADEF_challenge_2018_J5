#pragma once

#include <sstream>

class GlassItem {

    public:
    GlassItem();
    GlassItem(unsigned int w, unsigned int h, 
        unsigned int id, unsigned int id_stack, unsigned int stack_sequence): 
        m_w(w), m_h(h), m_id(id), m_id_stack(id_stack), m_stack_sequence(stack_sequence){}
    GlassItem(std::string input_line);
    
    unsigned int get_h() const;
    unsigned int get_h(bool rotated) const;
    unsigned int get_w() const;
    unsigned int get_w(bool rotated) const;
    unsigned int get_id() const;
    unsigned int get_id_stack() const;
    unsigned int get_area() const;
    unsigned int get_stack_sequence() const;

    private:
    unsigned int m_w;
    unsigned int m_h;
    unsigned int m_id;
    unsigned int m_id_stack;
    unsigned int m_stack_sequence;
    
};

std::ostream& operator<<(std::ostream& os, const GlassItem item);