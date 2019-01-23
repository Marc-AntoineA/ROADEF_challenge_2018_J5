#include <iostream>
#include <ctime>

using namespace std;

#include "glass_algorithm.h"


int main(int argc, char* argv[])
{
    // Check the number of parameters
    int time_limit = -1;
    string instance_name = "";
    string output_name = "";
    int seed = -1;
    for (int i = 0; i < argc; i++){
        if (string(argv[i]).compare("-t") == 0)
            time_limit = stoi(argv[i + 1]);
        if (string(argv[i]).compare("-p") == 0)
            instance_name = argv[i + 1];
        if (string(argv[i]).compare("-name") == 0){
            std::cout << "J5" << std::endl;
            return 1;
        }
        if (string(argv[i]).compare("-o") == 0)
            output_name = argv[i + 1];
        if (string(argv[i]).compare("-s") == 0)
            seed = stoi(argv[i + 1]);
    }

    std::cout << "ROADEF - Qualification Submission - Team J5" << std::endl;
    std::cout << "Parameters parsed : " << std::endl;
    std::cout << "\t time_limit :" << time_limit << std::endl;
    std::cout << "\t instance_name : " << instance_name << std::endl;
    std::cout << "\t output_name : " << output_name << std::endl;

    /*GlassAlgorithm algorithm(instance_name);    
    //algorithm.solve_heuristic();
    //algorithm.solve_depth_search(1);
    ///algorithm.solve_branch_and_bound();
    algorithm.reset();
    clock_t begin = std::clock();
    algorithm.solve_branch_and_bound(.99*time_limit, -1, 10);

    //algorithm.reset();
    //algorithm.solve_branch_and_bound(30, 1, 10);

    int elapsed_time = double(clock() - begin) / CLOCKS_PER_SEC;
    //algorithm.reset();
    //algorithm.solve_branch_and_bound(.99*(time_limit - elapsed_time), 1, 5);

    std::cout << "Temps passé : " << double(clock() - begin) / CLOCKS_PER_SEC << " s" << std::endl;

    //std::cout << "Temps passé : " << elapsed_time << "s" << std::endl;
    std::cout << "Score atteint : " << algorithm.get_best_score() << std::endl;
    //algorithm.log();
    algorithm.save(output_name);
    */
    return 0;
}