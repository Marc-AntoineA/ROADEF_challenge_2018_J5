#include "glassPlate.h"

#include "../GlassData/glassConstants.h"

#include <iostream>
#include <algorithm>

void GlassPlate::addDefect(GlassDefect defect) {
    defects.push_back(defect);
}

bool GlassPlate::rectangleIsFreeOutOfDefects(unsigned int x, unsigned int y, 
    unsigned int xw, unsigned int yh) const {
    
    for (const GlassDefect& defect : defects) {
        if (x < defect.getXWidth() && defect.getXWidth() <= xw 
            && y < defect.getYHeight() && defect.getYHeight() <= yh)
            return false;
        
        if (x <= defect.getX() && defect.getX() <= xw 
            && y < defect.getYHeight() && defect.getYHeight() <= yh)
            return false;
      
        if (x <= defect.getX() && defect.getX() <= xw
            && y <= defect.getY() && defect.getY() <= yh)
            return false;

        if (x < defect.getXWidth() && defect.getXWidth() <= xw 
            && y <= defect.getY() && defect.getY() <= yh)
            return false;
    }
    return true;
}

bool GlassPlate::cutIsFreeOutOfDefects(unsigned int x, unsigned int y, unsigned int size, bool cutIsVertical) const {
    for (const GlassDefect& defect: defects) {
        if (defect.intersectLine(x, y, size, cutIsVertical)){
            return false;
        }
    }
    return true;
}

unsigned int GlassPlate::getBestY(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const {    
    // Déterminer pour tous les défauts présents dans la marge, s'il y a la place de mettre
    // une pièce en hauteur, et à quel y ?
    unsigned int yMin = HEIGHT_PLATES;
    for (const GlassDefect& defect : defects) {
        if (defect.getXWidth() <= x) continue; // Défaut trop en amont
        if (defect.getX() > x + width) break; // Défaut trop en aval
        if (defect.getYHeight() <= y) continue; // Défaut trop bas

        // On cherche le défaut juste au dessus (et dans la borne) x est fixé
        unsigned int nextY = HEIGHT_PLATES;
        for (const GlassDefect& defect2 : defects) {
            if (defect2.getY() <= defect.getYHeight() && defect2.getYHeight() > defect.getYHeight()) {
                nextY = defect2.getYHeight();
                break;
            }
            if (defect2.getXWidth() <= x) continue; // Défaut trop en amont
            if (defect2.getX() > x + width) break; // Défauts trop en aval
            if (defect2.getYHeight() < defect.getYHeight()) continue; // Défaut trop bas
            if (defect2.getY() < nextY)
                nextY = defect2.getY();
        }
        // Rq nextY = HEIGHT_PLATES s'il n'y a pas de défauts
        if (nextY  > height + defect.getYHeight()){
            // Il y a de la place de mettre un élément au milieu
            yMin = std::min(yMin, defect.getYHeight());
        }
    }

    // Écart à y (avant le 1er défaut)
    unsigned int nextY = HEIGHT_PLATES;
    for (const GlassDefect& defect: defects){
        if (defect.getXWidth() <= x) continue; // ce défaut n'est pas assez près
        if (defect.getX() > x + width) break; // Les défauts après sont trop loin
        if (defect.getYHeight() <= y) continue;
        if (defect.getY() < nextY) nextY = defect.getY();
    }

    if (nextY > height + y) {
        yMin = std::min(yMin, y);
    }

    return yMin;
}

void GlassPlate::sortDefects() {
    std::sort(defects.begin(), defects.end());
}