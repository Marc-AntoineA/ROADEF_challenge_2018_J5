#include "algorithm.h"

#include <string>
#include <iostream>

using namespace std;


Algorithm::Algorithm(string instance){
   
    name_instance = instance;
    stacks = parseBatch(instance + "_batch.csv");
    plates = parseDefects(instance + "_defects.csv");
    cout << plates[0] << endl;
    cout << plates[97] << endl;
}














