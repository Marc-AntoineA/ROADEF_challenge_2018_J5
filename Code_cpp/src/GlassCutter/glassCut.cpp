#include "glassCut.h"

GlassCut::GlassCut() : abscissa(0), itemSequencePosition(0), verticalCut(VERTICAL_CUT), beginItem(BEGIN_ITEM)  {

}

GlassCut::GlassCut(unsigned int abscissa, unsigned int itemSequencePosition, bool beginItem, bool verticalCut) 
    : abscissa(abscissa), itemSequencePosition(itemSequencePosition), beginItem(beginItem), verticalCut(verticalCut) {

}

bool GlassCut::operator<(const GlassCut& other) const {
    if (abscissa == other.getAbscissa()) {
        if (beginItem == other.isBegin()) 
            return itemSequencePosition < other.getItemSequencePosition();
        return !beginItem;
    }
    return abscissa < other.getAbscissa();
}