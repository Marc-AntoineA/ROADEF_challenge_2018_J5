#include <iostream>
#include <ctime>
#include <cstdlib>

#include "../GlassData/glassInstance.h"
#include "../GlassKernel/heuristic.h"

using namespace std;

int main(int argc, char* argv[])
{
    srand(4);
    GlassInstance instance("instances_checker/A3");
    Heuristic heuristic(instance);
    heuristic.displaySequence();
    
    return 0;
}