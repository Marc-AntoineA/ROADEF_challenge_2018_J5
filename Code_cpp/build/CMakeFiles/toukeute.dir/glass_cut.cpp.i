# 1 "/home/marc-antoinea/Documents/4-Projets_et_realisations/ROADEF/Code cpp/glass_cut.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/marc-antoinea/Documents/4-Projets_et_realisations/ROADEF/Code cpp/glass_cut.cpp"
# 1 "/home/marc-antoinea/Documents/4-Projets_et_realisations/ROADEF/Code cpp/glass_cut.h" 1
       


class GlassCut {
    public:
    GlassCut();
    GlassCut(unsigned int x, unsigned int item, bool begin, bool vertical)
        : m_vertical(vertical), m_begin(begin), m_x(x), m_item(item) {}
    bool operator<(const GlassCut &cut) const;

    unsigned int get_x() const { return m_x; }
    bool get_begin() const { return m_begin; }
    bool get_vertical() const { return m_vertical; }
    unsigned int get_item() const { return m_item; }

    private:
    bool m_vertical;
    bool m_begin;
    unsigned int m_x;
    unsigned int m_item;
};
# 2 "/home/marc-antoinea/Documents/4-Projets_et_realisations/ROADEF/Code cpp/glass_cut.cpp" 2

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
