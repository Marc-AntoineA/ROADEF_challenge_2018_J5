#include "algorithm.h"

#include <string>
#include <iostream>
#include <stack>

using namespace std;


Algorithm::Algorithm(const string &instance){
    name_instance = instance;
    stacks = parseBatch(instance + "_batch.csv");
    plates = parseDefects(instance + "_defects.csv");
}

// L'appel de cette fonction suppose que sol contient une solution pafaitement bien constituée
// un appel à fixSolution sera peut-être nécessaire (à coder)
void Algorithm::export_solution(string filename){
    if (filename == "")
        filename = name_instance + "_solution.csv";
    ofstream write(filename.c_str());
    write << "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT" <<endl;
    for (vector<GlassNode>::iterator it = sol.begin(); it < sol.end(); it++)
        write << it->toString() << endl;
}

