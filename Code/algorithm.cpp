#include "algorithm.h"

#include <string>
#include <iostream>

using namespace std;


Algorithm::Algorithm(string instance){
   
    name_instance = instance;
    stacks = parseBatch(instance + "_batch.csv");
    plates = parseDefects(instance + "_defects.csv");
}

void Algorithm::glouton(method m){
    
}

/**
 * Renvoie la position (x, y) où placer un item orienté pour un node donné 
 * (le plus à gauche en bas possible)
 * output: false si aucun emplacement possible
 * Complexité = algo intelligent pour la répartition en Y mais pas en X
**/
bool Algorithm::position_defects(const GlassNode& node, const GlassItem& item, bool rotated, int& x, int& y){
    // Initialisation, piece en bas à gauche
    x = node.Getpos_x(); 
    y = node.Getpos_y();

    // Dimensions de la pièce
    int w = item.Getitem_w(rotated);
    int h = item.Getitem_h(rotated);

    // Dimension du noeud
    int WW = node.Getwidth();
    int HH = node.Getheight();

    int minY = y;
    int maxX = x + WW - w + 1;
    int maxY = y + HH - h + 1;
    int y_defect = y; // Plus grande ordonnée du défaut qui bloque la piece
    
    vector<GlassDefect>* defects = plates[node.Getnode_id()].Getdefects();
    bool location_found = false;
    while(!location_found && x < maxX && y < maxY){
        for(int defect_idx = 0; defect_idx < plates[node.Getnode_id()].Getdefect_nbr(); defect_idx++){
            if(intersect(x, y, w, h, (*defects)[defect_idx])){
                y_defect = (*defects)[defect_idx].Getpos_ySup();                
            }
        }
        // Si aucun défaut n'est présent
        if(y_defect == y){
            location_found = true;
        
        }else{
            y = y_defect;
            if(y >= maxY){
                y = minY;
                y_defect = y;
                x++;
            }
        }
    }
    return location_found;
}

bool Algorithm::intersect(const int& x, const int& y, const int& w, const int& h, const GlassDefect& defect){
    int XX = defect.Getpos_x();
    int YY = defect.Getpos_y();
    int WW = defect.Getwidth();
    int HH = defect.Getheight();

    return (XX >= x && XX < x + w && YY >= y && YY < y + h) // coin inférieur gauche du défaut
        || (XX + WW > x && XX + WW <= x + w && YY >= y && YY < y + h) // coin inférieur droit
        || (XX >= x && XX < x + w && YY + HH > y && YY + HH <= y + h) // coin superieur gauche
        || (XX + WW > x && XX + WW <= x + w && YY + HH > y && YY + HH <= y + h); // coin superieur droit
}