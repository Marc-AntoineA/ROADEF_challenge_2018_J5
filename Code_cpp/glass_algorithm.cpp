#include "glass_algorithm.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <stack>
#include <ctime>

#include "glass_stack.h"
#include "glass_node.h"

GlassAlgorithm::GlassAlgorithm (std::string instance_name) {
    
    instance = Instance(instance_name);
    build_stacks();
    current_bin_id = 0;
    monsters.push_back(RedMonster(instance.get_plate(0)));
    items_cuts.resize(1);
    nodes.push_back(GlassNode(instance.get_plate(0)));
    nb_locations_tried = 0;
    
    best_score = 100*widthPlates*heightPlates;
    x_max = 100*widthPlates;
    max_lost_area = 100*widthPlates*heightPlates;
    /*x_max = 5040;
    max_lost_area = 1467925;
    best_score = 1467925;*/

    // Log
    nb_rollbacks = 0;
    nb_failed = 0;
    nb_obviously_pruned = 0;
    nb_pruned = 0;

    used_area = 0;
    lost_area = 0;
}

void GlassAlgorithm::build_stacks() {
    
    std::cout << "Construction des stacks - en cours " << std::endl;
    stacks.clear();

    for (unsigned int i = 0; i < instance.get_nb_items(); ++i) {

        GlassItem* item = instance.get_item(i);
        unsigned int stack_id = item->get_id_stack();
        if (stacks.size() <= stack_id){
            stacks.resize(stack_id + 1);
            stacks_available.insert(stack_id);
        }
        stacks[stack_id].push(item);
    }
    std::cout << "Construction effectuée " << std::endl;
}

bool GlassAlgorithm::contains_stack_available(int s) {
    return stacks_available.find(s) != stacks_available.end();
}
 
void GlassAlgorithm::add_stack_available(int s) {
    stacks[s].push();
    if (stacks_available.find(s) == stacks_available.end()) stacks_available.insert(s);
}

GlassItem* GlassAlgorithm::remove_stack_available(int s) {
    if (stacks[s].empty()) { throw std::runtime_error("s empty"); }
    GlassItem* item = stacks[s].pop();
    if (stacks[s].empty()) stacks_available.erase(s);
    return item;
}

// TODO : calcul du score à partir de 0 ici
int GlassAlgorithm::get_score_from_scratch() {
    int x_max_tmp = 0;
    for (Location location: items_cuts[current_bin_id]){
        x_max_tmp = std::max(location.get_xw(), x_max_tmp);
    }

    int area = instance.get_area();
    return (current_bin_id*widthPlates + x_max_tmp)*heightPlates - area;
}

void GlassAlgorithm::save(std::string output_filename) {
    std::cout << "Sauvegarde de l'instance " << instance.get_instance_name() << std::endl;
    std::ofstream output_file;
    //output_file.open(PATH + instance.get_instance_name() + "_" + std::to_string(best_score) + "_solution.csv");
    output_file.open(output_filename);
    output_file << "PLATE ID;NODE ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT" << std::endl;


    int last_bin_id = 0;
    for (unsigned int bin_id=0; bin_id < best_items_cuts.size(); bin_id++){
        if (best_items_cuts[bin_id].empty()) break;
        last_bin_id++;
    }
    int max_son_id = -1;
    for (unsigned int bin_id=0; bin_id < last_bin_id; bin_id++){
        nodes[bin_id].build_node(best_items_cuts[bin_id].begin(), best_items_cuts[bin_id].end());
        max_son_id = nodes[bin_id].save_node(output_file, max_son_id + 1, -1, bin_id, bin_id == last_bin_id -1);
    }

    std::cout << "Sauvegarde effectuée avec succès " << std::endl;
}

bool GlassAlgorithm::attempt(Location& location) {
    nb_locations_tried++;
    //std::cout << "Tentative : " << location << std::endl;
    remove_stack_available(location.get_item()->get_id_stack());

    current_bin_id = location.get_bin_id();
    //std::cout << current_bin_id << std::endl;
    used_area += location.get_item()->get_area();

    //RedMonster copy = monsters[current_bin_id];// TODO remove
    monsters[current_bin_id].incr_red_monster(location);
    add_item_cuts(location);
    try {
        //nodes[current_bin_id].check_node(items_cuts[current_bin_id].begin(), items_cuts[current_bin_id].end());
        nodes[current_bin_id].build_node(items_cuts[current_bin_id].begin(), items_cuts[current_bin_id].end());
        assert(check_tree_feasability());
    } catch (const std::runtime_error& e){
        nb_failed++;
        //std::cout << items_cuts[current_bin_id].size() << std::endl;
        //std::cout << e.what() << std::endl;
        rollback();
        return false;
    } 
    return true;
}

int GlassAlgorithm::get_score_location(Location& location){
    int w_x = 3000;
    int w_y = 800; 
    int w_s = 30;

    if (location.get_yh() > 2900 && location.get_xw() > 5700) 
        return 1000000 - location.get_item()->get_area();
    if (location.get_yh() > 2900) 
        return location.get_xw();
    if (location.get_xw() > 5700) 
        return location.get_yh();

    return monsters[current_bin_id].get_area() 
        - w_s*location.get_item()->get_area() 
        + w_x*location.get_xw()
        - w_y*location.get_yh();
}
    

void GlassAlgorithm::rollback() {
    nb_rollbacks++;
    //std::cout << current_bin_id << std::endl;
    add_stack_available(pop_item_cuts());
    assert(check_next_monsters_empty());
}

int GlassAlgorithm::pop_item_cuts() {
    // NOTE ce sont des stacks pour items_cuts qui m'intéressent. 
    while (items_cuts[current_bin_id].size() == 0){
        if (current_bin_id == 0) throw std::runtime_error("Trop de pop");
        current_bin_id--;
    }
    int s = items_cuts[current_bin_id].back().get_item()->get_id_stack();
    used_area -= items_cuts[current_bin_id].back().get_item()->get_area();
    items_cuts[current_bin_id].pop_back();
    monsters[current_bin_id].rollback();

    if (current_bin_id > 0 && items_cuts[current_bin_id].empty()){
        monsters[current_bin_id].reset();
        current_bin_id--;
    }
    return s;
}

void GlassAlgorithm::add_item_cuts(Location location) {
    items_cuts[current_bin_id].push_back(location);
} 

std::vector<Location> GlassAlgorithm::get_locations(GlassItem* item, bool force, int depth) {
    if(current_bin_id < 0) throw std::runtime_error("current_bin_id < 0"); // CHECKER
    std::vector<Location> locations_tmp =  monsters[current_bin_id].get_locations(item); 
    std::vector<Location> locations;   
    int bin_id = current_bin_id;
    for (Location location: locations_tmp) {
        location.set_bin_id(bin_id);
        if (!attempt(location)) continue;
        //location.set_score(get_deep_score(depth));
        location.set_score(location.compute_score() + monsters[current_bin_id].get_area() - 4*location.get_item()->get_area());
        rollback();
        locations.push_back(std::move(location));
    }
    //for (Location& location: locations){ std::cout << location << std::endl; }
    if (force && locations.size() == 0) {
       incr_bin_id();
       return get_locations(item, force, depth);
    }
    if (VERBOSE){
        std::cout << "Emplacements possibles : " << std::endl;
        for (Location location: locations){
            std::cout << "\t" << location << std::endl;
        }
    }
    return locations;
}

std::vector<Location> GlassAlgorithm::get_locations(bool force, int depth) {
    std::vector<Location> locations;
    if (stacks_available.empty()) return locations;
    int bin_id = current_bin_id;
    for (int s = 0; s < stacks.size(); s++){
        if (stacks[s].empty()) continue;
        current_bin_id = bin_id;
        GlassItem* item = stacks[s].top();
        std::vector<Location> locations_tmp = get_locations(item, true, depth);
        locations.insert(locations.end(), locations_tmp.begin(), locations_tmp.end());
    }
    for (Location& location: locations){
        //std::cout << location << std::endl;
    }
    current_bin_id = bin_id;
    if (force && locations.empty()){
        incr_bin_id();
        return get_locations(force, depth);
    }
    return locations;
}

void GlassAlgorithm::incr_bin_id(){
    current_bin_id++;
    if (current_bin_id >= 100) throw new  std::runtime_error("Trop d'incrémentations de bins");
    if (current_bin_id >= monsters.size()){
        monsters.push_back(RedMonster(instance.get_plate(current_bin_id)));
        nodes.push_back(GlassNode(instance.get_plate(current_bin_id)));
    }
    if (current_bin_id >= items_cuts.size())
        items_cuts.resize(current_bin_id + 1);
    monsters[current_bin_id].reset();
    //std::cout << current_bin_id << std::endl;
}

void GlassAlgorithm::solve_test(){
    std::cout << "Construction de l'opt de la solution 'A1'" << std::endl;
    // Solution A1 hardcodée
    Location item0(instance.get_item(0), 0, 0, false, 0);
    Location item1(instance.get_item(1), 0, 1578, true, 1);
    Location item2(instance.get_item(2), 758, 0, true, 2);
    Location item3(instance.get_item(3), 758, 276, true, 3);
    Location item4(instance.get_item(4), 758, 1672, false, 4);

    items_cuts.push_back(std::vector<Location>());
    items_cuts[0].push_back(item0);
    items_cuts[0].push_back(item1);
    items_cuts[0].push_back(item2);
    items_cuts[0].push_back(item3);
    items_cuts[0].push_back(item4);
    std::cout << "Construction effectuée" << std::endl;
}

// Test de l'algorithme de découpe avec une solution hardcodée pour l'instance A4
void GlassAlgorithm::solve_test_A4(){
    std::cout << "Construction de l'opt de la solution 'A4'" << std::endl;
    // Solution A1 hardcodée
    Location item0(instance.get_item(0), 0, 0, true, 0);
    Location item1(instance.get_item(1), 0, 1142, false, 1);
    Location item2(instance.get_item(2), 1142, 0, false, 2);
    Location item3(instance.get_item(3), 1142, 902, false, 3);
    Location item4(instance.get_item(4), 1142, 1418, false, 4);
    Location item5(instance.get_item(5), 2046, 1418, false, 5);
    Location item6(instance.get_item(6), 2700, 0, false, 6);
    Location item7(instance.get_item(7), 2700, 848, true, 7);
    Location item8(instance.get_item(8), 2700, 1952, false, 8);
    Location item9(instance.get_item(9), 3288, 130, false, 9);
    std::cout << item0 << std::endl;
    std::cout << item1 << std::endl;
    std::cout << item2 << std::endl;
    std::cout << item3 << std::endl;
    std::cout << item4 << std::endl;
    std::cout << item5 << std::endl;
    std::cout << item6 << std::endl;
    std::cout << item7 << std::endl;
    std::cout << item8 << std::endl;
    std::cout << item9 << std::endl;
    items_cuts.push_back(std::vector<Location>());
    items_cuts[0].push_back(item0);
    items_cuts[0].push_back(item1);
    items_cuts[0].push_back(item2);
    items_cuts[0].push_back(item3);
    items_cuts[0].push_back(item4);
    items_cuts[0].push_back(item5);
    items_cuts[0].push_back(item6);
    items_cuts[0].push_back(item7);
    items_cuts[0].push_back(item8);
    items_cuts[0].push_back(item9);

    std::cout << "Construction effectuée" << std::endl;
}

void GlassAlgorithm::solve_test_random() {
    std::cout << "Début du test random" << std::endl;

    while (!stacks_available.empty()) {
        if (VERBOSE) std::cout << current_bin_id << std::endl;
        if (VERBOSE) std::cout << monsters[current_bin_id] << std::endl;
        int s = *stacks_available.begin();
        GlassItem* item = stacks[s].top();

        std::vector<Location> locations = get_locations(item, true, -1); // peut-être gérer avec un pointeur ici
       
        auto it = locations.begin();
        while (it != locations.end() && !attempt(*it)){
            it++;
        }
        if (it == locations.end()){ 
            incr_bin_id();
        }else {
            std::cout << *it << std::endl;
        }
    }
    std::cout << "Solution trouvée par le test" << std::endl;
}

void GlassAlgorithm::solve_heuristic(){
    std::cout << "Début du solveur par heuristique " << std::endl;

    while (!stacks_available.empty()){
        Location best_location;
        int best_area = heightPlates*widthPlates;
        int area;
        std::vector<Location> locations = get_locations(false, -1);
        for (Location location: locations){
            if (attempt(location)){
                area = get_score_location(location);
                if (VERBOSE) std::cout << "\ts: " << area << std::endl;
                if (area < best_area){
                    best_area = area;
                    best_location = location;
                }
                rollback();
            }
        }
        if (VERBOSE) std::cout << "And the winner is ..." << std::endl;
        if (best_area < heightPlates*widthPlates){
            if (VERBOSE) std::cout << "Emplacement choisi : " << best_location << " | s = ";
            if (VERBOSE) std::cout << get_score_location(best_location) << std::endl;
            attempt(best_location);
            if (VERBOSE) std::cout << monsters[current_bin_id] << std::endl;
        } else {
            incr_bin_id();
        } 
    }
    update_best_state();
}

void GlassAlgorithm::log(){
    std::cout << "Nombre d'emplacements testés : " << nb_locations_tried << std::endl;
    std::cout << "Nombre d'emplacements invalides : " << nb_failed << std::endl;
    std::cout << "Nombre de rollbacks : " << nb_rollbacks << std::endl;
}

void GlassAlgorithm::reset(){
    for (int stack_id = 0; stack_id < stacks.size(); stack_id++) {
        stacks[stack_id].reset();
        stacks_available.insert(stack_id);
    } 
    current_bin_id = 0;
    monsters.clear();
    monsters.push_back(RedMonster(instance.get_plate(0)));
    items_cuts.clear();
    items_cuts.resize(1);
    nodes.clear();
    nodes.push_back(GlassNode(instance.get_plate(0)));
    nb_locations_tried = 0;
    nb_rollbacks = 0;
    nb_failed = 0;
    nb_obviously_pruned = 0;
    nb_pruned = 0;

    used_area = 0;
    lost_area = 0;
}

void GlassAlgorithm::solve_depth_search(int max_depth){
     std::cout << "Début du solveur par profondeur " << std::endl;

    while (!stacks_available.empty()){
        Location best_location;
        int best_score = 100*widthPlates * heightPlates;
        int score;
        std::vector<Location> locations = get_locations(true, -1);
        for (Location& location: locations){
            if (attempt(location)){
                score = get_deep_score(max_depth);
                if (score < best_score){
                    best_score = score;
                    best_location = location;
                }
                rollback();
            }
        }
        if (VERBOSE) std::cout << "And the winner is ..." << std::endl;
        if (best_score < 100*heightPlates*widthPlates){
            std::cout << "Emplacement choisi : " << best_location << " | s = ";
            std::cout << best_score << std::endl;
            attempt(best_location);
            if (VERBOSE) std::cout << monsters[current_bin_id] << std::endl;
        } else {
            incr_bin_id();
        } 
    }
    update_best_state();
}

int GlassAlgorithm::get_current_score(){
    if (instance.get_nb_items() - nb_items < 3)
        return monsters[current_bin_id].get_x_max();
    int monster_area = monsters[current_bin_id].get_area();
    int lost_area = current_bin_id*heightPlates*widthPlates + monster_area - used_area;

    return lost_area
            - used_area*1.1
            + 100*monsters[current_bin_id].get_margin();// favoriser les grandes pièces)
}

int GlassAlgorithm::get_deep_score(int depth){
    
    if (depth < 0) 
        return get_current_score();

    std::vector<Location> locations = get_locations(false, -1);
    int score = 100*heightPlates*widthPlates;
    if (locations.empty()) 
        return get_current_score();

    //std::cout<< locations.size() << std::endl;
    for (Location& location: locations){
        //std::cout << location << std::endl;
        
        if (obviously_pruned(location)) {
            location.set_score(100*heightPlates*widthPlates);            
        }else {
            location.set_score(get_score_location(location));
        }
    }
    std::sort(locations.begin(), locations.end());
    int n = std::min((int)locations.size(), 10);
    for (int i = 0; i < n; i++){

        if (!attempt(locations[i])) { n = std::min(n + 1, (int)locations.size()); continue;}
        if (pruned()) {
            score =  100*heightPlates*widthPlates;
            rollback();
            continue;
        }
    
        score = std::min(get_deep_score(depth - 1), score);

        if (VERBOSE) std::cout << "score " << score << std::endl;
        rollback();
        /*std::cout << " --> " << 
        std::cout << "cb " << current_bin_id << std::endl;
        std::cout << k << std::endl;*/
    }
    return score;
}

bool GlassAlgorithm::pruned(){
    if (current_bin_id*widthPlates*heightPlates + monsters[current_bin_id].get_area() 
        - used_area + 0.02*(instance.get_area() - used_area) > max_lost_area) {
    return true;}
    return false;
}

bool GlassAlgorithm::obviously_pruned(const Location& location) const {
    if (current_bin_id*widthPlates + location.get_xw() > x_max) { 
        //std::cout << x_max << " O " << current_bin_id*widthPlates  << "-- " << location.get_xw();
        //std::cout << " " << location << std::endl; 
        return true;}

    return false;
}

void GlassAlgorithm::update_best_state(){
    int score = get_score_from_scratch();
    if (score < best_score){
        try {
            for (unsigned int bin_id=0; bin_id < items_cuts.size(); bin_id++){
                if (items_cuts[bin_id].empty()) break;
                nodes[bin_id].build_node(items_cuts[bin_id].begin(), items_cuts[bin_id].end());
            }
            best_score = score;
            best_items_cuts = items_cuts;// TODO warning copy
            x_max = current_bin_id*widthPlates + monsters[current_bin_id].get_x_max();
            //std::cout << x_max << std::endl;
            max_lost_area = x_max*heightPlates - instance.get_area();
            std::cout << "Score obtenu : " << best_score << " | nb tentatives : " << nb_locations_tried << std::endl;
        } catch (const std::runtime_error& e){std::cout << e.what() << std::endl;}
    }
}


bool GlassAlgorithm::check_nb_items(){
    int real_nb_items = 0;
    for (int bin_id = 0; bin_id <= current_bin_id; bin_id++){
        real_nb_items += monsters[bin_id].get_nb_items();
    }
    if (nb_items != real_nb_items){
        std::cout << real_nb_items << " != " << nb_items << std::endl;
        
        for (RedMonster monster: monsters){
            std::cout << monster << std::endl;
        }
        for (int bin_id = 0; bin_id <= current_bin_id; bin_id++){
            std::cout << "bin#" << bin_id << std::endl;
            for (Location location: items_cuts[bin_id]){
                std::cout << location << std::endl;
            }
        }
    }
    return real_nb_items == nb_items;
}

bool GlassAlgorithm::check_used_area_less_than_monster_area(){
    if (used_area <= current_bin_id*widthPlates*heightPlates + monsters[current_bin_id].get_area()) return true;
    for (Location location: items_cuts[current_bin_id]){
        std::cout << location << std::endl;
    }
    std::cout << monsters[current_bin_id] << std::endl;
    std::cout << used_area << " / " <<  current_bin_id*widthPlates*heightPlates + monsters[current_bin_id].get_area() << std::endl;
    return false;    
}

bool GlassAlgorithm::check_used_area() {
    int real_used_area = 0;
    for (int i = 0; i <= current_bin_id; i++){
        for (Location location: items_cuts[i]){
            real_used_area += location.get_item()->get_area();
        }
    }
    if (real_used_area == used_area) return true;
    for (int bin_id = 0; bin_id < current_bin_id; bin_id++){
        for (Location location: items_cuts[bin_id]){
            std::cout << location << " --area : " << location.get_item()->get_area() << std::endl;
        }
    } 
}

bool GlassAlgorithm::check_tree_feasability(){
    try {
        nodes[current_bin_id].check_node(items_cuts[current_bin_id].begin(), items_cuts[current_bin_id].end());
        return true;
    } catch (const std::runtime_error& e){
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool GlassAlgorithm::check_next_monsters_empty(){
    for (int i = current_bin_id + 1; i < monsters.size(); i++){
        if (monsters[i].get_nb_items() > 0) return false;
    }

    return true;
}
void GlassAlgorithm::solve_branch_and_bound(int time_limit, int depth, int rate){
    std::cout << "Début de l'algorithme de branch & bound " << std::endl;
    nb_items = 0;

    int nb_pushed = 0;
    int nb_poped = 0;
    int max_items = instance.get_nb_items();
    std::stack<Location> unseen;
    std::vector<Location> locations = get_locations(true, depth);
    std::cout << locations.size() << std::endl;
    std::sort(locations.rbegin(), locations.rend());
    for (Location location: locations) {
        location.set_item_pos(0); 
        //std::cout << "\t" << location << std::endl;
        nb_pushed++;
        unseen.push(std::move(location));
    }
    
    clock_t begin = std::clock();
    // TODO rajouter une contrainte de temps
    while (double(clock() - begin) / CLOCKS_PER_SEC < time_limit && !unseen.empty()){
        
        assert(check_used_area());
        assert(check_used_area_less_than_monster_area());
    
        //std::cout << "Params: nb_items : " << nb_items << " & bin_id " << current_bin_id << std::endl;
        Location location = unseen.top();
        //std::cout << "À tester : " << location << std::endl;
        //std::cout << monsters[current_bin_id] << std::endl;
        assert(location.get_item_pos() <= nb_items);
        //std::cout << nb_items << " " << location.get_item_pos() << std::endl;
        unseen.pop();
        //std::cout << "Après : " << unseen.top() << std::endl;
        if (location.get_item_pos() < nb_items){
            unseen.push(std::move(location));
            //std::cout << "ROLLBACK, un item en trop" << std::endl;
            rollback();
            nb_items--;
            continue;
        } 
        if (!check_nb_items())
        std::cout << location << std::endl;
        assert(check_nb_items());
        current_bin_id = location.get_bin_id();       
        //assert(location.get_bin_id() == current_bin_id);
        nb_poped++;
        if (obviously_pruned(location)){
            //std::cout << "obv pruned" << std::endl;
            nb_obviously_pruned++;
            continue;
        } 

        if (!attempt(location)){
            //std::cout << "emplacement failed" << std::endl;
            continue;
        }

        nb_items++;
        if (pruned()){
            //std::cout << "prune " << location << " " << best_score << std::endl;
            nb_pruned++;
            rollback();
            nb_items--;
            continue;
        }

        if (stacks_available.empty()){
            
            update_best_state();
            nb_items--;
            rollback();
            continue;
        }

        locations = get_locations(true, depth);
        std::sort(locations.rbegin(), locations.rend());
        int n = std::min((int)locations.size(), rate);

        for (int i = 0; i < n; i++){
            int j = locations.size() - n + i;
            locations[j].set_item_pos(nb_items);
            //std::cout << location.get_score() << std::endl;
            unseen.push(std::move(locations[j]));
            nb_pushed++;
        }

        if (nb_locations_tried % 100000 == 0){
            std::cout << nb_locations_tried  << " / " << unseen.size();
            std::cout<< " -op : " << nb_obviously_pruned;
            std::cout<< " -p : " << nb_pruned;
            std::cout<< " -nb : " << nb_items;
            std::cout<< " -bin : " << location.get_bin_id()<<std::endl;
        }
    }
    //assert(nb_pushed == nb_poped);
    std::cout << nb_pushed << " / " << nb_poped << std::endl;
    std::cout << unseen.empty() << std::endl;
}