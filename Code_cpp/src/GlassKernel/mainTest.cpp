
#include "../GlassData/glassInstance.h"
#include "../GlassKernel/heuristic.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <boost/thread.hpp>

void solveThread(Heuristic& heuristic) {
    std::cout << "Un nouveau thread vient de partir " << std::endl;
    //heuristic.start();
}

using namespace std;

int main(int argc, char* argv[])
{
    unsigned int nbThreads = 4;
    unsigned int timeLimit = 60;
    unsigned int depthLimit = 4;
    string instanceName = "";
    string outputName = "";
    int seed = 0;
    for (int i = 0; i < argc; i++){
        if (string(argv[i]).compare("-t") == 0)
            timeLimit = std::stoi(argv[i + 1]);
        if (string(argv[i]).compare("-p") == 0)
            instanceName = argv[i + 1];
        if (string(argv[i]).compare("-o") == 0)
            outputName = argv[i + 1];
        if (string(argv[i]).compare("-s") == 0)
            seed = std::stoi(argv[i + 1]);
        if (string(argv[i]).compare("-d") == 0) 
            depthLimit = std::stoi(argv[i + 1]);
        if (string(argv[i]).compare("-n") == 0)
            nbThreads = std::stoi(argv[i + 1]);
        if (string(argv[i]).compare("-name") == 0){
            std::cout << "J5" << std::endl;
            return 1;
        }
    }

    std::cout << "Params: " << std::endl;
    std::cout << " - instanceName: " << instanceName << std::endl;
    std::cout << " - outputName: " << outputName << std::endl;
    std::cout << " - timeLimit: " << timeLimit << std::endl,
    std::cout << " - NbThreads: " << nbThreads << std::endl;
    std::cout << " - depthLimit: " << depthLimit << std::endl;
    std::cout << " - seed: " << seed << std::endl;

    srand(seed); // TODO --> qu'est-ce qu'on en fait ?
    
    std::vector<Heuristic> heuristics;

    for (unsigned int threadIndex = 0; threadIndex < nbThreads; threadIndex++) {
        GlassInstance instance("instances_checker/" + instanceName);
        Heuristic heuristic(&instance, timeLimit, depthLimit);
        heuristics.push_back(heuristic);
    }

    /*std::vector<std::thread> threads;
    for (unsigned int threadIndex = 0; threadIndex < nbThreads; threadIndex++) {
        threads.push_back(std::thread(solveThread(heuristics[threadIndex])));
    }

    for (unsigned int threadIndex = 0; threadIndex < nbThreads; threadIndex++) {
        threads[threadIndex].join();
    }*/
    
    return 0;
}