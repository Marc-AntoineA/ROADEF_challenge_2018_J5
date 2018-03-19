#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <stdint-gcc.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Static Path to global_parameters, X_batch, X_defects and X_solution files
#define PATH_TO_INSTANCES           "instances_checker\\"

// Plates number limit
#define PLATES_NBR_LIMIT            100

/*---------------------------------------------------------------*/
/*-------------------  csv files configuration ------------------*/
/*---------------------------------------------------------------*/

// csv files colonnes number
#define BATCH_COL_LENGTH            6
#define SOLUTION_COL_LENGTH         9
#define DEFECTS_COL_LENGTH          6

// Optimization parameters raws number
#define OPT_PARAM_RAWS_LENGTH       8

// Batch file colonnes index
#define BATCH_ITEM_ID_COL           0
#define BATCH_LENGTH_ITEM_COL       1
#define BATCH_WIDTH_ITEM_COL        2
#define BATCH_STACK_COL             3
#define BATCH_SEQ_COL               4

// Solution file colonnes index
#define SOLUTION_PLATE_ID_COL       0
#define SOLUTION_NODE_ID_COL        1
#define SOLUTION_X_COL              2
#define SOLUTION_Y_COL              3
#define SOLUTION_WIDTH_COL          4
#define SOLUTION_HEIGHT_COL         5
#define SOLUTION_TYPE_COL           6
#define SOLUTION_CUT_COL            7
#define SOLUTION_PARENT_COL         8

// Defects file colonnes index
#define DEFECTS_DEFECT_ID_COL       0
#define DEFECTS_PLATE_ID_COL        1
#define DEFECTS_X_COL               2
#define DEFECTS_Y_COL               3
#define DEFECTS_WIDTH_COL           4
#define DEFECTS_HEIGHT_COL          5

// Optimization parameters file raws index
#define OPT_PARAM_NPLATES_RAW             0
#define OPT_PARAM_WIDTHPLATES_RAW        1
#define OPT_PARAM_HEIGHTPLATES_RAW         2
#define OPT_PARAM_MIN1CUT_RAW             3
#define OPT_PARAM_MAX1CUT_RAW             4
#define OPT_PARAM_MIN2CUT_RAW             5
#define OPT_PARAM_MINWASTE_RAW       6

/*---------------------------------------------------------------*/
/*--------  Solution's possible errors mask and offset  --------*/
/*---------------------------------------------------------------*/

// Mask and Offset for errors found while parsing solution file.
#define PARSE_ERROR_MASK                    1
#define PARSE_ERROR_OFFSET                  0

// Mask and Offset for errors for item production constraint verification.
#define ITEM_PRODUCTION_ERROR_MASK          2
#define ITEM_PRODUCTION_ERROR_OFFSET        1

// Mask and Offset for errors for defects superposing constraint verification.
#define DEFECTS_SUPERPOSING_ERROR_MASK      4
#define DEFECTS_SUPERPOSING_ERROR_OFFSET    2

// Mask and Offset for errors for items dimensions constraint verification.
#define DIMENSIONS_ERROR_MASK               8
#define DIMENSIONS_ERROR_OFFSET             3

// Mask and Offset for errors for identity constraint verification.
#define IDENTITY_ERROR_MASK                 16
#define IDENTITY_ERROR_OFFSET               4

// Mask and Offset for errors for sequence constraint verification.
#define SEQUENCE_ERROR_MASK                 32
#define SEQUENCE_ERROR_OFFSET               5

unsigned int active_log = 1; // Once log file is generated this var will be cleared.
ofstream log_file; // Reference to log file.

ofstream statistics_file; // Reference to statisticsLog file.
bool opened_file = true; // Boolean to test if one of needed files (batch, solution, defects and optimization parameters) was not opened.

string file_idx; // To save used file index (Normal mode).
string testPath; // To save path according to chosen test.
string instance; // To save used file index (Test mode).

unsigned int s_idx = 0; // Solution index, used precise where to insert solution node in sol_items array.
unsigned int plates_nbr = 0; // Save solution used plates number.
unsigned int stack_nbr = 0; // Save batch stack number.
unsigned int plates_list [PLATES_NBR_LIMIT][2];   // Save number of nodes for each plate used in user solution.
                                                  // plates_list[X][Y]:   X dimension contains plates Ids list
                                                  //                      Y dimension contains number of nodes of plate X
unsigned int lines_nbr = 0; // csv file lines number.
unsigned int node_nbr = 0; // Solution file nodes number.
unsigned int useful_node = 0; // Solution useful nodes number (node not a branch or waste or residual).
unsigned int branch_node = 0; // Solution branch nodes number
unsigned int residual_node = 0; // Solution residual nodes number.
unsigned int waste_node = 0; // Solution waste nodes number.
unsigned int batch_items = 0; // Batch items number.
unsigned int constraint_error = 0; // Success constraint, combination of solution's occured errors (refer to main.h to find errors list).
unsigned int defects_nbr = 0; // Defects file defects number.

unsigned int total_waste = 0; // Sum of wasted area.
unsigned int total_useful = 0; // Sum of used area.

int max_cut_stage = 0; // Solution max used cut stage.

unsigned int plate_nbr_limit = 0; // To save Optimization parameters plate number limit constraint.
unsigned int plate_w = 0; // To save Optimization parameters plate's length constraint.
unsigned int plate_h = 0; // To save Optimization parameters plate's width constraint.
unsigned int min1Cut = 0; // To save Optimization parameters cut 1 & 2 min1Cut constraint.
unsigned int min2Cut = 0; // To save Optimization parameters cut 1 & 2 min2Cut constraint.
unsigned int max1Cut = 0; // To save Optimization parameters cut 1 & 2 max1Cut  constraint.
unsigned int waste_min = 0; // To save Optimization parameters cut 1 & 2 minWasteWidth constraint.

unsigned int success = 1;

#endif // GLOBALVAR_H
