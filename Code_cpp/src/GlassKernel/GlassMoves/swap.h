#pragma once

#include "../glassMove.h"

class Swap: public GlassMove {
    public:
    void attempt();
    void commit();
    void revert();

    private:
    unsigned int firstIndex;
    unsigned int lastIndex;
};