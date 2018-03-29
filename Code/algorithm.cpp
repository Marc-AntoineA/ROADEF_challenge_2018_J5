#include "algorithm.h"

#include <string>
#include <iostream>
#include <stack>

using namespace std;


Algorithm::Algorithm(string instance){
   
    name_instance = instance;
    stacks = parseBatch(instance + "_batch.csv");
    plates = parseDefects(instance + "_defects.csv");
}

bool Algorithm::decision(const GlassNode& node, GlassItem& item, bool& rotated, int& x, int& y, method n){
    if (n==basic){
        for (int i=0;i<stacks.size();i++){
            if (!stacks[i].Depleted()){
                item = stacks[i].Top();
                if (position_defects(node, item, rotated, x, y))
                    return true;
            }
        }
        return false;
    }
    //if (n == otherMethod){
    //....
    //}
}

void Algorithm::cut(const GlassItem &item, GlassNode& node, const bool &rotated, const int &x, const int &y, stack<GlassNode> nodes_to_visit){
    int alpha = node.Getcut();
    alpha = (alpha == 0)? 1 : alpha;
    if (!rotated){
        int h = item.Getitem_h();
        int w = item.Getitem_w();
    }
    else{
        int h = item.Getitem_w();
        int w = item.Getitem_h();
    }
    int H = node.Getheight();
    int W = node.Getwidth();
    if (alpha % 2 == 0){
        if (h == H)
            cut_horizontally(node, y, stack<GlassNode> nodes_to_visit);
    }
}

void Algorithm::glouton(method m){
    stack<GlassNode> nodes_to_visit;
    int x, y; bool rotated;
    GlassNode current_node;
    GlassItem current_item;
    while (thereAreItemsToCut || !nodes_to_visit.empty()){
        if (nodes_to_visit.empty()){

        }
        current_node = nodes_to_visit.top();
        nodes_to_visit.pop();
        if (decision(current_node, current_item, rotated, x, y, m)){
            cut(current_item, current_node, rotated, x, y, nodes_to_visit);
        }
    }
}

/**
 * Renvoie la position (x, y) où placer un item orienté pour un node donné 
 * (le plus à gauche en bas possible)
 * output: false si aucun emplacement possible
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
