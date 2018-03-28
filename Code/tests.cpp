
/**
 * Executable de test pour s'assurer du bon fonctionnement 
 * du code. 
 * Procédures à lancer régulièrement 
**/

#include "global_var.h"
#include "algorithm.h"

#include <vector>
using namespace std;

bool test_position_defects(){
    cout << "Test_position_defects..." << endl;
    bool results = true;
    
    Algorithm A = Algorithm("../Datasets/dataset_A/A6");
    GlassNode node = GlassNode();    
    node.Setwidth(plate_w);
    node.Setheight(plate_h);

    vector<GlassStack> stacks = A.Getstacks();
    vector<GlassPlate> plates = A.Getplates();

    /* Plate 0 (defects)
    Id 0	 H 3210	 W 6000
	    Id 0	Plate 0	W 5	H 2	x 1135	y 694
	    Id 1	Plate 0	W 5	H 6	x 5280	y 2987 
    */
    int x, y;
    GlassItem item = stacks[0].Getitem(4);
    A.position_defects(node, item, false, x, y);
    results = results && (x == 0 && y == 0);
    if(!(x == 0 && y == 0)){
        cout << "\t\tError test 1 x : " << x << " and y :" << y << endl;
    }
    
    item.Setitem_w(1665); // pour etre sur de rencontrer le defaut 0
    item.Setitem_h(694); // juste en dessous
    A.position_defects(node, item, false, x, y);
    results = results && (x == 0 && y == 0);
    if(!(x == 0 && y == 0)){
        cout << "\t\tError test 2 x : " << x << " and y :" << y << endl;
    }
    

    item.Setitem_h(695); 
    A.position_defects(node, item, false, x, y);
    results = results && (x == 0 && y == 696);
    if(!(x == 0 && y == 696)){
        cout << "\t\tError test 3 x : " << x << " and y :" << y << endl;
    }

    // pour la rotation
    item.Setitem_h(1000);
    item.Setitem_w(600);
    A.position_defects(node, item, true, x, y);
    results = results && (x == 0 && y == 0);
    if(!(x == 0 && y == 0)){
        cout << "\t\tError test 4 x : " << x << " and y :" << y << endl;
    }

    // pour le deplacement selon x
    item.Setitem_h(plate_h);
    item.Setitem_w(3000);
    A.position_defects(node, item, false, x, y);
    results = results && (x == 1140 && y == 0);
    if(!(x == 1140 && y == 0)){
        cout << "\t\tError test 5 x : " << x << " and y :" << y << endl;
    }

    // trop grand
    item.Setitem_h(plate_h);
    item.Setitem_w(plate_w + 1);
    results = results && !A.position_defects(node, item, true, x, y);
    if(A.position_defects(node, item, false, x, y)){
        cout << "\t\tError test 6 x" << endl;
    }

    if(!results){
        cout << "\t...Test failed" << endl;
    }else{
        cout << "\t...Test Ok" << endl;
    }
    return results;
}



int main(void)
{
    test_position_defects();
}
