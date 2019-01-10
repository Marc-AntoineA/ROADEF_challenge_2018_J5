#include "glassPlate.h"

void GlassPlate::addDefect(GlassDefect defect) {
    defects.push_back(defect);
}

bool GlassPlate::rectangleIsFreeOutOfDefects(unsigned int x, unsigned int y, 
    unsigned int xw, unsigned int yh) const {
    
    for (const GlassDefect& defect : defects) {
        if (x < defect.getXWidth() && defect.getXWidth() < xw 
            && y < defect.getYHeight() && defect.getYHeight() <= yh)
            return false;
        
        if (x <= defect.getX() && defect.getX() <= xw 
            && y < defect.getYHeight() && defect.getYHeight() <= yh)
            return false;
      
        if (x <= defect.getX() && defect.getX() <= xw
            && y < defect.getY() && defect.getY() <= yh)
            return false;

        if (x < defect.getXWidth() && defect.getXWidth() <= xw 
            && y <= defect.getY() && defect.getY() <= yh)
            return false;
    }
    return true;
}