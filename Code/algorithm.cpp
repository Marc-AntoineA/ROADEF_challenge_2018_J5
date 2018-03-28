#include "algorithm.h"

#include <string>
#include <iostream>

using namespace std;


Algorithm::Algorithm(string instance){

    string batch_name = instance + "_batch.csv";
    cout << batch_name << endl;
    stacks = parseBatch(batch_name);
    cout << stacks.size() << endl;
    cout << stacks[2].Getitem(4) << endl;
}














