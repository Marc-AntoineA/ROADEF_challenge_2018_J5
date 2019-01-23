#pragma once

#include "instance.h"
#include "glass_stack.h"
#include "glass_item.h"
#include "red_monster.h"
#include "location.h"
#include "glass_node.h"

#include <string>
#include <set>
#include <vector>

class GlassAlgorithm {

    public:
    GlassAlgorithm(std::string instance_name);
    
    void solve_test();
    void solve_test_A4();
    void solve_test_random();
    void solve_heuristic();
    void solve_depth_search(int max_depth);
    void solve_branch_and_bound(int time_limit, int depth, int rate);
    int get_score_from_scratch();
    int get_best_score() const { return best_score; }
    void save(std::string output_file);
    void log();
    void reset();

    private:    
    GlassItem* remove_stack_available(int s);
    void add_stack_available(int s);
    void add_item_cuts(Location location);
    int pop_item_cuts();
    bool attempt(Location &location);
    int get_score_location(Location& location);
    void rollback();
    void build_stacks();
    bool contains_stack_available(int s);
    void incr_bin_id();
    int get_deep_score(int depth);
    int get_current_score();
    std::vector<Location> get_locations(GlassItem* item, bool force, int depth);
    std::vector<Location> get_locations(bool force, int depth);
    // peut-être avoir ici un pointeur pour maintenir la copie
    Instance instance;
    
    bool obviously_pruned(const Location& location) const;
    bool pruned();
    void update_best_state();

    std::vector<GlassStack> stacks;
    std::set<int> stacks_available;
    // Notre solution : les vitres coupées par bin
    unsigned int current_bin_id; // between 0 and 100
    std::vector<std::vector<Location> > items_cuts;
    std::vector<RedMonster> monsters;
    std::vector<GlassNode> nodes;

    // Checkers
    int nb_items;
    bool check_used_area();
    bool check_used_area_less_than_monster_area();
    bool check_nb_items();
    bool check_tree_feasability();
    bool check_next_monsters_empty();

    // Log
    int nb_locations_tried;
    int nb_rollbacks;
    int nb_failed;
    int nb_obviously_pruned;
    int nb_pruned;

    // Score
    int used_area;
    int lost_area;

    // Best solution
    int x_max;
    int best_score;
    int max_lost_area;
    std::vector<std::vector<Location>> best_items_cuts;
};