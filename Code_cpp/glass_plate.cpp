#include "glass_plate.h"

#include <algorithm>
#include <cassert>

#include "instance.h"

GlassPlate::GlassPlate() {
    defects = std::vector<GlassDefect>();
}

GlassPlate::GlassPlate(int plate_id): m_plate_id(plate_id){
    defects = std::vector<GlassDefect>();
}

void GlassPlate::add_defect(GlassDefect defect){
    defects.push_back(defect);
    std::sort(defects.begin(), defects.end());
}

int GlassPlate::get_plate_id() const{
    return m_plate_id;
}

void GlassPlate::set_plate_id(int plate_id) {
    m_plate_id = plate_id;
}

// TODO avoir un mapping des x qui posent problème
bool GlassPlate::is_free_out_of_defects(int x, int y, int h, bool vertical) {
    // Typiquement, x, y, w et h correspondent à une coupe (GlassCut)
    
    for (GlassDefect& defect: defects) {
        if (false){
            std::cout << x << ", " << y << ", " << h << ", " << vertical << std::endl;
            std::cout << defect;
        }
        if (defect.intersect_line(x, y, h, vertical)){
            
            return false;
        }
    }
    return true;
}

/*std::vector<GlassDefect>::iterator GlassPlate::get_defect_margin(int xw, int w){

    auto defect_margin = defects.begin();
    while (defect_margin != defects.end() && defect_margin->get_x() - xw < w){
        defect_margin++;
    }
    return defect_margin;
}

std::vector<GlassDefect>::iterator GlassPlate::get_first_it(int x) {
    auto first_it = defects.begin();
    while (first_it != defects.end() && first_it->get_xw() < x){
        first_it++;
    }
    return first_it;
}*/

int GlassPlate::get_best_y(const Location &location, int current_x){

    // Déterminer pour tous les défauts présents dans la marge, s'il y a la place de mettre
    // une pièce en hauteur, et à quel y ?
    int y_min = heightPlates;            
    for (auto d1 = defects.begin(); d1 !=  defects.end(); d1++){        
        if (d1->get_xw() <= current_x) continue; // ce défaut n'est pas assez près
        if (d1->get_x() > current_x + location.get_w()) break; // Les défauts après sont trop loin
        if (d1->get_yh() <= location.get_y()) continue; // Le défaut est trop bas    
        
        // On cherche le défaut juste au dessus (et dans la borne) (x fixé)
        int next_y = heightPlates;
        for (auto d2 = defects.begin(); d2 != defects.end(); d2++){
            if (d2->get_y() <= d1->get_yh() && d2->get_yh() > d1->get_yh()){next_y = d2->get_yh(); break; }
            if (d2->get_xw() <= current_x) continue;
            if (d2->get_x() > current_x + location.get_w()) break; // Les défauts après sont trop loin
            if (d2->get_y() < d1->get_yh()) continue; // le défaut est trop bas
            if (d2->get_y() < next_y) next_y = d2->get_y(); 
        }

        // NB. next_y = heightPlates lorsque pas de défauts
        if (next_y - d1->get_yh() > location.get_h()) {
            // Il y a la place de mettre un élément au milieu
            y_min = std::min(y_min, d1->get_yh());
        }
    }

    // Écart à y (avant le 1er défaut)
    int next_y = heightPlates;
    for (auto d2 = defects.begin(); d2 != defects.end(); d2++){
        if (d2->get_xw() <= current_x) continue; // ce défaut n'est pas assez près
        if (d2->get_x() > current_x + location.get_w()) break; // Les défauts après sont trop loin
        if (d2->get_y() < next_y) next_y = d2->get_y();
    }
    if (next_y - location.get_y() > location.get_h()) {
        y_min = std::min(y_min, location.get_y());
    }

    /*std::cout << "Recherche du best y pour les paramètres : " << std::endl;
    std::cout << "\t Location : " << location << std::endl;
    std::cout << "\t x : " << current_x << std::endl;
    std::cout << "===> y_min = " << y_min << std::endl;*/
    return y_min;
}

std::vector<Location> GlassPlate::locations_free_out_of_defects(const Location &location) {
    std::vector<Location> locations;
    if (is_free_out_of_defects(location)) {
        locations.push_back(location); return locations;
    }

    int w = location.get_w();
    int h = location.get_h();

    int current_x = location.get_x();
    int y_min = get_best_y(location, current_x);
    
    if (y_min < heightPlates){
        locations.push_back(Location(current_x, y_min, location));
    }

    // ça ne sert à rien d'aller plus bas
    if (y_min == location.get_y()) return locations;

    int y_prev = y_min;
    auto defect_it = defects.begin();   
    // Initialiser avec x % location.get_x()
    while (defect_it != defects.end()) {
        current_x = defect_it->get_xw();
        if (current_x != location.get_x()) current_x = std::max(current_x, location.get_x() + 20);

        y_min = get_best_y(location, current_x);
        
        if (y_min > y_prev) break;
        if (y_min < heightPlates && y_min == y_prev) break;

        if (y_min < heightPlates){
            if (y_min != location.get_y()) y_min = std::max(y_min, location.get_y() + 20);
            locations.push_back(Location(current_x, y_min, location));
        } 
        //std::cout << location << std::endl;
        y_prev = y_min;        
        defect_it++;
    }

    // TODO remove 
    for (Location location:locations){
        if (!is_free_out_of_defects(location)){
            //std::cout << location << std::endl;
        }
        assert(is_free_out_of_defects(location));
    }
    return locations;
}

bool GlassPlate::is_free_out_of_defects(const Location& location) const{
    for (GlassDefect defect: defects){
        // Coin supérieur droit dans la pièce
        if (location.get_x() < defect.get_xw() && defect.get_xw() <= location.get_xw() 
            && location.get_y() < defect.get_yh() && defect.get_yh() <= location.get_yh()){
                //std::cout << defect << std::endl;            
                return false;
            }
        
        // Coin supérieur gauche
        if (location.get_x() <= defect.get_x() && defect.get_x() <= location.get_xw()
            && location.get_y() < defect.get_yh() && defect.get_yh() <= location.get_yh()){
                //std::cout << defect << std::endl;
                return false;
            }

        // Coin inférieur gauche
        if (location.get_x() <= defect.get_x() && defect.get_x() <= location.get_xw()
            && location.get_y() <= defect.get_y() && defect.get_y() <= location.get_yh()){
                //std::cout << defect << std::endl;
                return false;
            }
        
        // Coin inférieur droit
        if (location.get_x() < defect.get_xw() && defect.get_xw() <= location.get_xw()
            && location.get_y() <= defect.get_y() && defect.get_y() <= location.get_yh()){
                //std::cout << defect << std::endl;
                return false;
            }
    }
    return true;
}