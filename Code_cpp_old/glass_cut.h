#pragma once

// QUESTION  : est-ce que la coupe est absolue ?
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
    bool m_vertical; // true iff découpe verticale (en x)
    bool m_begin; // true iff début d'une pièce
    unsigned int m_x; // contient y si découpe horizontale
    unsigned int m_item; // emplacement de l'item.
};