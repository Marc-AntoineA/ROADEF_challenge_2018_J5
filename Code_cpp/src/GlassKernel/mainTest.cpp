
#include "../GlassData/glassInstance.h"
#include "../GlassKernel/heuristic.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define NB_THREADS_DEPTH_1 4
#define NB_THREADS_DEPTH_2 4
#define DEPTH_1 3
#define DEPTH_2 4
#define SAVING_TIME 8

void solveThread(Heuristic* heuristic) {
    heuristic->start();
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

    std::cout << std::endl;
    std::cout << "Challenge ROADEF/EURO 2018: Cutting Optimization Problem" << std::endl;
    std::cout << "Final Submission - Team J5 - Marc-Antoine Auge" << std::endl;
    std::cout << "Usage: ./exec -t timeLimit (in seconds) -o outputName -p instancePathAndName -s seed" << std::endl;

    std::cout << std::endl;
    std::cout << "Params: " << std::endl;
    std::cout << " - instanceName: " << instanceName << std::endl;
    std::cout << " - outputName: " << outputName << std::endl;
    std::cout << " - timeLimit: " << timeLimit << std::endl,
    //std::cout << " - NbThreads: " << nbThreads << std::endl;
    //std::cout << " - depthLimit: " << depthLimit << std::endl;
    std::cout << " - seed: " << seed << std::endl;
    std::cout << std::endl;

    unsigned int realTimeLimit = timeLimit > SAVING_TIME ? timeLimit - 10 : 1;
    srand(seed);

    std::vector<Heuristic*> heuristics;
    boost::thread_group group;
    std::vector<boost::thread*> threads;
    
    for (unsigned int threadIndex = 0; threadIndex < NB_THREADS_DEPTH_1; threadIndex++) {
        GlassInstance* instance = new GlassInstance(instanceName);
        heuristics.push_back(new Heuristic(instance, realTimeLimit, DEPTH_1, rand()));
        group.add_thread(new boost::thread(solveThread, heuristics[threadIndex]));
    }

    for (unsigned int threadIndex = 0; threadIndex < NB_THREADS_DEPTH_2; threadIndex++) {
        GlassInstance* instance = new GlassInstance(instanceName);
        heuristics.push_back(new Heuristic(instance, realTimeLimit, DEPTH_2, rand()));
        group.add_thread(new boost::thread(solveThread, heuristics[NB_THREADS_DEPTH_1 + threadIndex]));
    }

    group.join_all();
    unsigned int bestScore = std::numeric_limits<unsigned int>::max();
    unsigned int bestThreadIndex = 0;

    for (unsigned int threadIndex = 0; threadIndex < heuristics.size(); threadIndex++) {
        if (heuristics[threadIndex]->getBestScore() <= bestScore) {
            bestScore = heuristics[threadIndex]->getBestScore();
            bestThreadIndex = threadIndex;
            std::cout << bestScore << std::endl;
        }
    }

    std::cout << "Meilleur score obtenu: " << bestScore << std::endl;
    heuristics[bestThreadIndex]->saveAndComputeBest(outputName);
    //heuristics[bestThreadIndex]->displaySequence();
    //heuristics[bestThreadIndex]->displayLocations();
    
    return 0;
}