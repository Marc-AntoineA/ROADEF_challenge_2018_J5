#include "algorithm.h"
#include "checker.h"

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

bool Algorithm::checker_complete(){

    bool correct = true;

    cout << "Checker..." << endl;

    // verify sequence and identity
    bool identity_sequence = verifyIdt_Sequence(stacks, sol);
    if(identity_sequence){
        cout << "\t\tIdentity and sequence :D" << endl;
    }else{
        cout << "\t\tIdentity and sequence :'(" << endl;
    }
    correct = correct && identity_sequence;

    // Verify Item Production 
    bool item_production = verifyItemProduction(stacks, sol);
    if(item_production){
        cout << "\t\tItem production :D" << endl;
    }else{
        cout << "\t\tItem production :'(" << endl;
    }
    correct = correct && item_production;

    // Verify defects
    bool defects = verifyDefects(plates, sol);
    if(defects){
        cout << "\t\tDefects overlap :D" << endl;
    }else{
        cout << "\t\tDefects overlap :'(" << endl;
    }
    correct = correct && defects;

    // Verify dimensions
    bool dimensions = verifyDimensions(plates, sol);
    if(dimensions){
        cout << "\t\tDimensions :D" << endl;
    }else{
        cout << "\t\tDimensions :'(" << endl;
    }
    correct = correct && defects;
    
    return correct;
}