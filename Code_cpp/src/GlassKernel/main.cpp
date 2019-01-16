
#include "../GlassData/glassInstance.h"
#include "../GlassKernel/heuristic.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

int main(int argc, char* argv[])
{

    unsigned int timeLimit = 60;
    string instance_name = "";
    string output_name = "";
    int seed = 0;
    for (int i = 0; i < argc; i++){
        if (string(argv[i]).compare("-t") == 0)
            timeLimit = std::stoi(argv[i + 1]);
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
    seed = 0;
    instance_name = "A2";
    timeLimit = 1000;
    srand(seed);
    GlassInstance instance("/home/marc-antoinea/Documents/4-Projets_et_realisations/ROADEF/Code_cpp/src/build/bin/instances_checker/" + instance_name);
    Heuristic heuristic(&instance, std::round(timeLimit*.9), 4);
    heuristic.saveBest(instance_name + ".csv");
    //heuristic.localSearch();
    heuristic.displaySequence();
    
    return 0;
}