#include "class_initialization.h"

#include "glass_node.h"
#include "glass_stack.h"
#include "glass_item.h"
#include "glass_plate.h"
#include "vector"
#include "string"
#include <iostream>

/* ==================================================================================== */
/*              Different Class instantiation and initialization functions              */
/* ==================================================================================== */
/**
 * This function creates a plate at a given index of the
 * plate array used by the user solution.
 * token: input / string array contains plate parameters.
 * p: input / index at plate array
**//*
void createPlate(vector<GlassPlate> plates, string *token, int p)
{
    plate[p].Setplate_id(atoi(token[SOLUTION_PLATE_ID_COL].c_str()));
    plate[p].Setnode_id(atoi(token[SOLUTION_NODE_ID_COL].c_str()));
    plate[p].Setpos_x(atoi(token[SOLUTION_X_COL].c_str()));
    plate[p].Setpos_y(atoi(token[SOLUTION_Y_COL].c_str()));
    // if solution read value of plate length value is different from plate length optimization parameter value
    if(atoi(token[SOLUTION_WIDTH_COL].c_str()) != plate_w)
    {
        cout << "\tERROR --  Plate " << atoi(token[SOLUTION_PLATE_ID_COL].c_str()) << ": width not equal to widthPlates " << plate_w << endl;
        log_file << "\tERROR --  Plate " << atoi(token[SOLUTION_PLATE_ID_COL].c_str()) << ": width not equal to widthPlates " << plate_w << endl;
        constraint_error |= PARSE_ERROR_MASK;
    }
    plate[p].Setwidth(atoi(token[SOLUTION_WIDTH_COL].c_str()));
    // if solution read value of plate width value is different from plate height optimization parameter value
    if(atoi(token[SOLUTION_HEIGHT_COL].c_str()) != plate_h)
    {
        cout << "\tERROR -- Plate " << atoi(token[SOLUTION_PLATE_ID_COL].c_str()) << ": height not equal to lengthPlates " << plate_h << endl;
        log_file << "\tERROR -- Plate " << atoi(token[SOLUTION_PLATE_ID_COL].c_str()) << ": height not equal to lengthPlates " << plate_h << endl;
        constraint_error |= PARSE_ERROR_MASK;
    }
    plate[p].Setheight(atoi(token[SOLUTION_HEIGHT_COL].c_str()));
    plate[p].Settype(atoi(token[SOLUTION_TYPE_COL].c_str()));
    plate[p].Setcut(atoi(token[SOLUTION_CUT_COL].c_str()));
    plate[p].successor = new GlassNode[plates_list[p][1]];
    plate[p].Setnodes_nbr(plates_list[p][1]);
    plate[p].Setwaste(0);
}


/**
 * This function creates a node at a given index of the
 * node array used by the user solution.
 * node: output / list in which function will create the node
 * token: input / string array contains node parameters.
 * n: input / index at node array
**/
void createNode(vector<GlassNode>& nodes, string *token, int n)
{
    nodes[n].Setplate_id(atoi(token[SOLUTION_PLATE_ID_COL].c_str()));
    nodes[n].Setnode_id(atoi(token[SOLUTION_NODE_ID_COL].c_str()));
    nodes[n].Setpos_x(atoi(token[SOLUTION_X_COL].c_str()));
    nodes[n].Setpos_y(atoi(token[SOLUTION_Y_COL].c_str()));
    nodes[n].Setwidth(atoi(token[SOLUTION_WIDTH_COL].c_str()));
    nodes[n].Setheight(atoi(token[SOLUTION_HEIGHT_COL].c_str()));
    nodes[n].Settype(atoi(token[SOLUTION_TYPE_COL].c_str()));
    nodes[n].Setcut(atoi(token[SOLUTION_CUT_COL].c_str()));
    nodes[n].Setparent(atoi(token[SOLUTION_PARENT_COL].c_str()));
}


/**
 * This function takes the node array given by the solution parser
 * and build a tree structure of plates and their successors
 * to build the first level of the tree structure.
 * node: input / node array.
**/
void createItem(vector<GlassItem>& items, string *token, int p)
{
    items[p].Setitem_id(atoi(token[BATCH_ITEM_ID_COL].c_str()));
    items[p].Setitem_h(atoi(token[BATCH_LENGTH_ITEM_COL].c_str()));
    items[p].Setitem_w(atoi(token[BATCH_WIDTH_ITEM_COL].c_str()));
    items[p].Setitem_stack(atoi(token[BATCH_STACK_COL].c_str()));
    items[p].Setitem_seq(atoi(token[BATCH_SEQ_COL].c_str()));
}
