#pragma once

#define BEGIN_ITEM true
#define VERTICAL_CUT true

class GlassCut {
    public:
    GlassCut();
    GlassCut(unsigned int abscissa, unsigned int itemSequencePosition, bool beginItem, bool verticalCut);
    bool operator<(const GlassCut& other) const;
    unsigned int getAbscissa() const { return abscissa; }
    unsigned int getItemSequencePosition() const { return itemSequencePosition; }
    bool isBegin() const { return beginItem; }
    bool isVerticalCut() const { return verticalCut; }

    private:
    unsigned int abscissa;
    unsigned int itemSequencePosition;
    bool verticalCut;
    bool beginItem;
};