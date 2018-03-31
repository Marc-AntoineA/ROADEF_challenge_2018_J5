#pragma once

#include <string>
#include <vector>
#include <stack>

#include "glass_node.h"
#include "glass_stack.h"
#include "glass_item.h"
#include "glass_item.h"
#include "glass_plate.h"

#include "global_var.h"
#include "parser.h"
#include "algorithm.h"
#include <iostream>
using namespace std;

class Greedy: public Algorithm{
    
public:

    // Le constructeur prend en entrée le nom de l'instance courante à étudier
    Greedy(const string& instance): Algorithm(instance){}


    // ---------------------------------------------
    //            GLOUTON

    // met à jour les attributs avec un algorithme glouton
    void run(const method &m = basic);

    // Renvoie la meilleure position en bas à gauche pour une pièce (variables par références x et y)
    // et une orientation (rotated == true si on regarde la version pivotée de pi/2)
    // les contraintes. False si aucune solution possible
    bool position_defects(const GlassNode& node, const GlassItem& item, bool rotated, int& x, int& y);
    bool intersect(const int& x, const int& y, const int& w, const int& h, const GlassDefect& defect);


    // Calcule un score selon un critère (paramètre avec l'enumerate method - voir global_var.h)
    float score(const GlassNode& node, const GlassItem& item, const bool& rotated, const int& x, const int& y,  method m = basic);

    // Met à jour la pièce à découper (current_item) et renvoie sa position en vue de la découpe
    // et renvoie false si aucune pièce n'est découpable dans le node
    bool decision(method meth);

    // Coupe la pièce courante selon la décision
    void cut();

    bool thereAreItemsToCut(){
        for (int i=0;i<stacks.size();i++){
            if (!stacks[i].Depleted())
                return true;
        }
        return false;
    }

protected:

    // Attributs particuliers du Greedy
    stack<GlassNode> nodes_to_visit;
    GlassNode current_node;
    GlassItem current_item;
    int current_bin;
    bool rotated;
    int x;
    int y;
    int node_id = -1;

    int getNewNodeId(){
        node_id++;
        return node_id;
    }
};
