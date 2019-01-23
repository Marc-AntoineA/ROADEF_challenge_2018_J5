#include "red_monster.h"

#include <iostream>
#include <cassert>

RedMonster::RedMonster(GlassPlate* pl){
    // TODO peut-être un bug de constructeur, à creuser
    nb_items = 0;
    plate = pl;
}

RedMonster::RedMonster(){
    nb_items = 0;
}

void RedMonster::reset(){
    nb_items = 0;
    points.clear();
    old_points = std::priority_queue<RedPoint, std::vector<RedPoint>, sortSequenceClass>();
}

void RedMonster::incr_red_monster(const Location& location) {
    
    nb_items++;
    //std::cout << "Incrementation : " << nb_items << " (nb d'items)" << std::endl;
    clean(location.get_x(), location.get_y());
    //std::cout << "cleaned" << std::endl;
    RedPoint rp(location.get_xw(), location.get_yh(), nb_items);
    //std::cout << "Incr etoile verte " << std::endl;
    incr_green_star(rp);
    /*for (RedPoint rp: points){
        std::cout << rp.get_x() << " : " << rp.get_y() << " | " << rp.get_item_sequence() << std::endl;
    }*/
}

void RedMonster::incr_green_star(RedPoint new_point) {
    unsigned int k = 0;

    while (k < points.size()){
        RedPoint rp = points[k];
        
        if (rp.get_x() > new_point.get_x()){
            break;
        }

        if (rp.get_x() == new_point.get_x()) {
            RedPoint_kill(k);
            continue;
        }

        if (rp.get_y() <= new_point.get_y()) {
            RedPoint_kill(k);
            continue;
        }
        k++;
    }
    // Attention, cela fonctionne uniquement si les emplacements
    // vérifient bien la condition de l'étoile verte
    if ((k < points.size() && new_point.get_y() > points[k].get_y())
        || k == points.size())
        points.insert(points.begin() + k, new_point);
}

void RedMonster::RedPoint_kill(int k){
    points[k].kill(nb_items);
    //std::cout << "kill -- " << points[k] << std::endl;
    old_points.push(points[k]);
    points.erase(points.begin() + k);
}
void RedMonster::clean(int x, int y) {
    // TODO ça ne marche pas : revoir le clean et ses règles
    if (points.empty()) return;
    //int x_min = std::max(0, points.back().get_x() - 3500);
    int k = 0;
    while (k < (int) points.size() - 1) {
        // Si le plus gros élément est à gauche 
        if (points[k].is_after(points[k + 1])) {
            if (x > points[k].get_x() && y <= points[k + 1].get_y()) {
                RedPoint new_point = max(points[k], points[k + 1], nb_items);
                RedPoint_kill(k);
                RedPoint_kill(k);
                points.insert(points.begin() + k, new_point);
                k--;
           }
        } else {
            if (x <= points[k].get_x() && y >= points[k].get_y()) {
                RedPoint new_point = max(points[k], points[k+1], nb_items);
                RedPoint_kill(k);
                RedPoint_kill(k);
                points.insert(points.begin() + k, new_point);
                k--;
            }
        }
        k++;    
    }
}

bool RedMonster::check_location(const Location& location){
    if (location.get_yh() != heightPlates && location.get_yh() > heightPlates - 20) return false;
    if (location.get_xw() != widthPlates && location.get_xw() > widthPlates - 20) return false;
    if (location.get_x() != 0 && location.get_x() < 20) return false;
    if (location.get_y() != 0 && location.get_y() < 20) return false;
    if (!points.empty() && location.get_x() < points.back().get_x() - maxXX) return false;
    return true;
}

void RedMonster::add_locations_free_of_defects(std::vector<Location>& locations,const Location& location){
    if (!check_location(location)) return;
    std::vector<Location> locations_free_of_defects = plate->locations_free_out_of_defects(location);

    for (Location loc:locations_free_of_defects){
        locations.push_back(std::move(loc));
    }
}

std::vector<Location> RedMonster::get_locations(GlassItem* item) {
    int w = item->get_w();
    int h = item->get_h();
    
    int x_max_rot = widthPlates - h;
    int y_max_rot = heightPlates - w;
    int x_max = widthPlates - w;
    int y_max = heightPlates - h;

    std::vector<Location> locations;
    int n = points.size();

    if (n == 0) {
        add_locations_free_of_defects(locations, Location(item, 0, 0, true, nb_items));
        add_locations_free_of_defects(locations, Location(item, 0, 0, false, nb_items));
        return locations;    
    }

    // TODO gérer le x qui avance (les - 400 &co)
    int x = 0;
    int y = points[0].get_y();
    if (y <= y_max) 
        add_locations_free_of_defects(locations, Location(item, x, y, false, nb_items));
    
    if (y <= y_max_rot) 
        add_locations_free_of_defects(locations, Location(item, x, y, true, nb_items));

    int k = 0;
    // TODO Etre plus précis sur la sélection de x et limiter les emplacements
    while (k < n - 1 && x < std::max(x_max, x_max_rot)) {
        x = points[k].get_x();
        y = points[k + 1].get_y();

        if (y <= y_max) 
            add_locations_free_of_defects(locations, Location(item, x, y, false, nb_items));
    
        if (y <= y_max_rot) 
            add_locations_free_of_defects(locations, Location(item, x, y, true, nb_items));

        k++;
    }
    x = points[points.size() - 1].get_x();
    y = 0;

    if (x <= x_max)
        add_locations_free_of_defects(locations, Location(item, x, y, false, nb_items));

    if (x <= x_max_rot) 
        add_locations_free_of_defects(locations, Location(item, x, y, true, nb_items));

    return locations;    
}   
    
void RedMonster::RedPoint_resurrection(RedPoint old_point) {
    int k = 0; 
    while (k < points.size() && points[k] < old_point){
        k++;
    }
    old_point.rebirth();
    //std::cout << "resurrection -- " << old_point;
    points.insert(points.begin() + k, old_point);
}

void RedMonster::rollback() {
    nb_items--;
    while (!old_points.empty() && old_points.top().get_death() > nb_items) {
        //std::cout << old_points.top() << std::endl;
        RedPoint_resurrection(old_points.top());
        old_points.pop();
    }
    unsigned int k = 0;
    while (k < points.size()){
        if (points[k].get_birth() > nb_items) {
            points.erase(points.begin() + k);
        }else{
            k++;
        }
    }
}

bool RedMonster::operator==(RedMonster& other){
    if (nb_items != other.get_nb_items()) return false;
    if (points.size() != other.get_points()->size()) return false;
    for (int k = 0; k < points.size(); k++){
        if (points[k] != *(other.get_points()-> begin() + k)) return false;
    }
    return true;
}


int RedMonster::get_area(){
    if (points.empty()) return 0;

    int x_max = points.back().get_x();
    int area = 0;
    int x = std::max(x_max - 3500, 0);
    area += x*heightPlates;
    unsigned int k = 0;
    while (k < points.size() && points[k].get_x() < x_max - 3500){ k++; }
    
    while (k < points.size()){
        if (points[k].get_y() > heightPlates - 100)
            area += (points[k].get_x() - x)*heightPlates;
        else
            area += (points[k].get_x() - x)*points[k].get_y();
        x = points[k].get_x();
        k++;
    }
    k = points.size() - 1;
    if (points[k].get_x() > widthPlates - 100) {
        assert((widthPlates - points[k].get_x())*heightPlates >= 0);
        assert((widthPlates - points[k].get_x())*points[k].get_y() >= 0);
        if (points[k].get_y() > heightPlates - 100)
            area += (widthPlates - points[k].get_x())*heightPlates;
        else
            area += (widthPlates - points[k].get_x())*points[k].get_y();
    }
    
    return area;
}

int RedMonster::get_margin(){
    return heightPlates*get_x_max() - get_area();
}

int RedMonster::get_x_max() const {
    if (points.empty()) return 0;
    return points.back().get_x();
}

std::ostream& operator<<(std::ostream& os, RedMonster& monster){
    os << "RedMonster #" << monster.get_plate()->get_plate_id() << " -- ";
    os << monster.get_points()->size() << "pt(s) ";
    os << "-- aire : " << monster.get_area() << " ";
    os << monster.get_nb_items() << " items :" << std::endl;
   
   for (auto it = monster.get_points()->begin(); it != monster.get_points()->end(); it++){
        std::cout << "\t" << *it;
    }
    return os;
}
