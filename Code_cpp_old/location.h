#pragma once

#include "glass_item.h"

#include <iostream>
#include <sstream>

class Location {

    public:
    Location(){};
    Location(GlassItem* item, int x, int y, bool rotated, int(sequence))
        :m_x(x), m_y(y), m_item(item), m_rotated(rotated), m_sequence(sequence) {
            m_item_pos = -1;
             m_bin_id = 0; 
             m_score = 0;
    }

    Location(int x, int y, const Location& location)
        :m_x(x), 
        m_y(y),
        m_item(location.get_item()), 
        m_rotated(location.get_rotated()),
        m_sequence(location.get_sequence()){
        m_item_pos = -1; 
        m_bin_id = 0; 
    }

    int get_w() const { return m_item->get_w(m_rotated); }
    int get_h() const { return m_item->get_h(m_rotated); }
    int get_x() const { return m_x; }
    int get_xw() const { return m_x + m_item->get_w(m_rotated); }
    int get_y() const { return m_y; }
    int get_yh() const { return m_y + m_item->get_h(m_rotated); }
    int get_sequence() const { return m_sequence; }
    int get_rotated() const { return m_rotated; }
    int get_item_pos() const { return m_item_pos; }
    void set_item_pos(int item_pos) { m_item_pos = item_pos; }
    void set_score(int score) { m_score = score; }
    bool operator<(const Location& loc) const;
    GlassItem* get_item() const { return m_item; }
    int get_score() const { return m_score; }
    int compute_score() const;
    int get_bin_id() const { return m_bin_id; }
    void set_bin_id(int bin_id) { m_bin_id = bin_id; }

    private:
    int m_x;
    int m_y;
    GlassItem* m_item; // Attention, un pointeur vers l'item
    bool m_rotated;
    int m_sequence;
    int m_item_pos; // en partie redondant avec item_sequence. TODO refactor
    int m_bin_id;
    int m_score;
};

std::ostream& operator<<(std::ostream& os, const Location& location);