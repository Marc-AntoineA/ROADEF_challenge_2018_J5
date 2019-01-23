#include "glass_cut.h"

GlassCut::GlassCut() {
    m_x = 0;
    m_vertical = true;
    m_begin = true;
    m_item = 0;
}
    

bool GlassCut::operator<(const GlassCut &cut) const {
    if (m_x == cut.m_x) {
        if (m_begin == cut.m_begin) {
            return m_item < cut.m_item;
        }
        return !m_begin;
    }
    return m_x < cut.m_x;
}

