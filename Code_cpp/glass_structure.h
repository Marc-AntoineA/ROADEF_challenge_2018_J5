#ifndef GLASS_STRUCTURE_H
#define GLASS_STRUCTURE_H

#include "instance.h"
#include <vector>

class GlassStructure {
    
    public:
        GlassStructure(Instance *instance): instance(instance) {}
        unsigned int get_score();
        void dummy_init_locations();
    
    private:
        std::vector<int> locations;
        Instance* instance;
    
    
};

#endif /* GLASS_STRUCTURE_H */

