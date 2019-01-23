
#include "../GlassData/glassInstance.h"
#include "../GlassKernel/heuristic.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

void solveThread(Heuristic* heuristic) {
    heuristic->start();
}

void test() {
    std::cout << "On tente sans mettre d'argument, juste pour voir ce que cela donne" << std::endl;
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

    /*seed = 541;
    instanceName = "B13";
    outputName = "B13.csv";
    timeLimit = 20;
    nbThreads = 4;
    depthLimit = 3;*/
    srand(seed);

    std::vector<Heuristic*> heuristics;

    for (unsigned int threadIndex = 0; threadIndex < nbThreads; threadIndex++) {
        GlassInstance* instance = new GlassInstance("/home/marc-antoinea/Documents/4-Projets_et_realisations/ROADEF/Code_cpp/src/build/bin/instances_checker/" + instanceName);
        heuristics.push_back(new Heuristic(instance, timeLimit, depthLimit, rand()));
    }   

    boost::thread_group group;
    std::vector<boost::thread*> threads;
    for (unsigned int threadIndex = 0; threadIndex < nbThreads; threadIndex++) {
        group.add_thread(new boost::thread(solveThread, heuristics[threadIndex]));
    }

    group.join_all();
    unsigned int bestScore = std::numeric_limits<unsigned int>::max();
    unsigned int bestThreadIndex = 0;

    for (unsigned int threadIndex = 0; threadIndex < nbThreads; threadIndex++) {
        if (heuristics[threadIndex]->getBestScore() <= bestScore) {
            bestScore = heuristics[threadIndex]->getBestScore();
            bestThreadIndex = threadIndex;
            std::cout << bestScore << std::endl;
        }
    }
    std::cout << "Meilleur score obtenu: " << bestScore << std::endl;
    heuristics[bestThreadIndex]->saveBest(outputName);
    //heuristics[bestThreadIndex]->displayLocations();
    // TODO delete heuristics and threads
    
    return 0;
}