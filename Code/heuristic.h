#pragma once

#include <string>
#include <vector>

#include "glass_node.h"
#include "glass_stack.h"
#include "glass_item.h"
#include "glass_item.h"
#include "glass_plate.h"

#include "global_var.h"
#include "parser.h"

#include "greedy.h"

class Heuristic{
    
    public:

        // Le constructeur prend en entrée le glouton
        Heuristic(Greedy glouton);

        // renvoie la valeur du score correspondant à la déf. de la roadef 
        unsigned int get_score_roadef(); 

        // s'assure que toutes les contraintes sont vérifiées en utilisant le code fourni
        // a priori long mais pas besoin de l'utiliser en fonctionnement normal
        bool checker_complete();

        // Gestion du temps de calcul
        void init_cpu_time(); // Initialise un compteur
        int get_current_time(); // Renvoie la valeur actuelle du compteur

        // Gestion d'un fichier de log (résultats et paramètres pour faire des stats dessus)
        // Enregistre dans un fichier de log les résultats obtenus ainsi que des paramètres
        void log_results();

        // Ecrit dans des fichiers de manière cohérente la solution sol_items
        void export_solution();


        std::vector<GlassStack> Getstacks() const { return stacks; }
        std::vector<GlassPlate> Getplates() const { return plates; }
        std::vector<GlassItem> Getitems() const { return items; }
        std::vector<GlassNode> Getsol() const { return sol; }

    private:
        // Paramètres du problème
        std::string name_instance;
        std::vector<GlassPlate> plates;
        std::vector<GlassStack> stacks;
        std::vector<GlassItem> items;

        // Solution 
        std::vector<GlassNode> sol; // Cutting pattern
};
