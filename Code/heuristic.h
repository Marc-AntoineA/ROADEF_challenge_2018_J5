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
#include "algorithm"

class Heuristic: public Algorithm{
    
    public:

        // Le constructeur prend en entrée le glouton
        Heuristic(Greedy glouton);
};
