#include "heuristic.h"

#include <string>
#include <iostream>
#include <stack>

using namespace std;


Heuristic::Heuristic(Greedy glouton){
    name_instance = glouton.Getname_instance();
    plates = glouton.Getplates();
    stacks = glouton.Getstacks();
    items = glouton.Getitems();
    sol = glouton.Getsol();
}

