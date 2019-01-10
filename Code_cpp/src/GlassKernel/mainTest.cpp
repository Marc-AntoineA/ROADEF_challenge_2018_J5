#include <iostream>
#include <ctime>
#include <cstdlib>

#include "../GlassData/glassInstance.h"
#include "../GlassKernel/heuristic.h"

using namespace std;

int main(int argc, char* argv[])
{

    int time_limit = -1;
    string instance_name = "";
    string output_name = "";
    int seed = -1;
    for (int i = 0; i < argc; i++){
        if (string(argv[i]).compare("-t") == 0)
            time_limit = std::stoi(argv[i + 1]);
        if (string(argv[i]).compare("-p") == 0)
            instance_name = argv[i + 1];
        if (string(argv[i]).compare("-name") == 0){
            std::cout << "J5" << std::endl;
            return 1;
        }
        if (string(argv[i]).compare("-o") == 0)
            output_name = argv[i + 1];
        if (string(argv[i]).compare("-s") == 0)
            seed = std::stoi(argv[i + 1]);
    }
    srand(seed);
    GlassInstance instance("instances_checker/" + instance_name);
    Heuristic heuristic(instance);
    //heuristic.localSearch();
    heuristic.displaySequence();
    
    return 0;
}