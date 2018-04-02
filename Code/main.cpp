#include <iostream>
using namespace std;

#include "global_var.h"
#include "greedy.h"

int main(void)
{
    Greedy G("../Datasets/dataset_A/A6");
    G.run(simple_score);
    cout << "done" << endl;
    for (int i=0; i < G.Getsol().size(); i++){
        cout << G.Getsol()[i] << endl;
    }
    G.export_solution();
}
