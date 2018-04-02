#include "greedy.h"

#include <string>
#include <iostream>
#include <stack>
#include <cmath>
#include <algorithm> 

using namespace std;

bool Greedy::decision(method meth){
    if (meth == basic){
        for (int i=0;i<stacks.size();i++){
            if (!stacks[i].Depleted()){
                current_item = stacks[i].Top();
                //TODO : les aligner en mode paysage en priorité ?
                rotated = true;
                position_defects(current_node, current_item, rotated, x, y);
                if (position_defects(current_node, current_item, rotated, x, y)){
                    return true;
                }
                rotated = false;
                if (position_defects(current_node, current_item, rotated, x, y)){
                    return true;
                }
            }
        }
        return false;
    }
    //if (n == otherMethod){
    //....
    //}
}

void Greedy::cut(){
    int alpha = current_node.Getcut();
    alpha = (alpha == 0)? 1 : alpha;
    int h = current_item.Getitem_h(rotated);
    int w = current_item.Getitem_w(rotated);
    int H = current_node.Getheight();
    int W = current_node.Getwidth();
    int X = current_node.Getpos_x();
    int Y = current_node.Getpos_y();
    //on détermine la direction dans laquelle il faut couper.
    cutting_direction d;
    if (alpha % 2 == 0){
        if (h < H)
            d =horizontal;
        else if (h == H){
            d = vertical;
            alpha ++;
            // Les vraies branches sont ajoutées à la solution ici
            current_node.Settype(BRANCH);
            sol.push_back(current_node);
        }
    }
    if (alpha % 2 == 1){
        if (w < W)
            d = vertical;
        else if (w == W){
            d = horizontal;
            alpha ++;
            // Les vraies branches sont ajoutées à la solution ici
            current_node.Settype(BRANCH);
            sol.push_back(current_node);
        }
    }
    //on coupe vraiment
    GlassNode node;
    node.Setcut(alpha);
    node.Setplate_id(current_node.Getplate_id());
    node.Setparent(current_node.Getnode_id());
    if (d == vertical){
        node.Setheight(H);
        node.Setpos_y(Y);
        //S'il y a un noeud à droite
        if (x + w < X + W){
            node.Setpos_x(x + w);
            node.Setwidth(X + W - x - w);
            node.Setnode_id(getNewNodeId());

            nodes_to_visit.push(node);
        }
        // Le noeud du centre
        node.Setpos_x(x);
        node.Setwidth(w);
        node.Setnode_id(getNewNodeId());

        // on regarde si on ne vient pas de couper un nouvel item
        if (h == H){

            stacks[current_item.Getitem_stack()].Pop();

            // Les items découpés sont ajoutés à la solution ici

            node.Setnode_id(getNewNodeId());

            sol.push_back(node);
        }
        else
            nodes_to_visit.push(node);
        if (x - X > 0){
            node.Setpos_x(X);
            node.Setwidth(x - X);

            node.Setnode_id(getNewNodeId());

            nodes_to_visit.push(node);
        }
    }
    if (d == horizontal){
        node.Setwidth(W);
        node.Setpos_x(X);
        // S'il y a un noeud en haut
        if (y + h < Y + H){
            node.Setpos_y(y + h);
            node.Setheight(Y + H - y - h);

            node.Setnode_id(getNewNodeId());

            nodes_to_visit.push(node);
        }
        // le noeud du centre
        node.Setpos_y(y);
        node.Setheight(h);

        node.Setnode_id(getNewNodeId());

        // on regarde si on ne vient pas de couper un nouvel item
        if (w == W){
            stacks[current_item.Getitem_stack()].Pop();
            // Les items découpés sont ajoutés à la solution ici
            node.Settype(current_item.Getitem_id());
            sol.push_back(node);
        }
        else
            nodes_to_visit.push(node);
        // S'il y a un noeud en bas
        if (y - Y > 0){
            node.Setpos_y(Y);
            node.Setheight(y - Y);

            node.Setnode_id(getNewNodeId());

            nodes_to_visit.push(node);
        }
    }
}

void Greedy::run(const method& m){
    current_bin = -1;
    int alpha;
    while (thereAreItemsToCut() || !nodes_to_visit.empty()){
        if (nodes_to_visit.empty()){
            current_bin++;
            GlassNode plateNode;
            //alpha, X and Y already at zero by default
            plateNode.Setplate_id(current_bin);
            plateNode.Setnode_id(getNewNodeId());
            plateNode.Setwidth(plate_w);
            plateNode.Setheight(plate_h);
            plateNode.Settype(BRANCH);
            sol.push_back(plateNode);
            nodes_to_visit.push(plateNode);
        }
        current_node = nodes_to_visit.top();
        alpha = current_node.Getcut();
        //Pour vérifier la contrainte de profondeur
        nodes_to_visit.pop();
        decision(m);
        if (alpha <= 3 && decision(m)){ // il faudrait vérifier que le "et" est bien gourmand
            cut();
        }
        else{
            //Le waste est ajouté à la solution ici
            current_node.Settype(WASTE);
            sol.push_back(current_node);
        }
    }
    sol[sol.size()-1].Settype(RESIDUAL);
}


/**
 * Renvoie la position (x, y) où placer un item orienté pour un node donné 
 * (le plus à gauche en bas possible)
 * output: false si aucun emplacement possible
 * Complexité = algo intelligent pour la répartition en Y mais pas en X
**/
bool Greedy::position_defects(const GlassNode& node, const GlassItem& item, bool rotated, int& x, int& y){
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
    int x_defect = maxX; // Plus petite abscisse

    vector<GlassDefect>* defects = plates[node.Getplate_id()].Getdefects();
    bool location_found = false;

    while(!location_found && x < maxX && y < maxY){
        for(int defect_idx = 0; defect_idx < plates[node.Getplate_id()].Getdefect_nbr(); defect_idx++){
            if(intersect(x, y, w, h, (*defects)[defect_idx])){
                y_defect = max(y_defect, (*defects)[defect_idx].Getpos_ySup());                
                x_defect = min(x_defect, (*defects)[defect_idx].Getpos_xSup());
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
                x = x_defect;
            }
        }
    }
    return location_found;
}

bool Greedy::intersect(const int& x, const int& y, const int& w, const int& h, const GlassDefect& defect){
    int XX = defect.Getpos_x();
    int YY = defect.Getpos_y();
    int WW = defect.Getwidth();
    int HH = defect.Getheight();

    return (XX >= x && XX < x + w && YY >= y && YY < y + h) // coin inférieur gauche du défaut
        || (XX + WW > x && XX + WW <= x + w && YY >= y && YY < y + h) // coin inférieur droit
        || (XX >= x && XX < x + w && YY + HH > y && YY + HH <= y + h) // coin superieur gauche
        || (XX + WW > x && XX + WW <= x + w && YY + HH > y && YY + HH <= y + h); // coin superieur droit
}
