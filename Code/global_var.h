#pragma once

#include <stdint-gcc.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum method{ basic, simple_score };
enum cutting_direction{ horizontal, vertical };

const string LOGFILE = "/results.csv";

// Static Path to global_parameters, X_batch, X_defects and X_solution files
#define PATH_TO_INSTANCES           "../checker/instances_checker/glouton_results/"

// Plates number limit
#define PLATES_NBR_LIMIT            100

/*---------------------------------------------------------------*/
/*-------------------GLASS_NODE and GLASS_PLATE------------------*/
/*---------------------------------------------------------------*/

#define WASTE -1
#define BRANCH -2
#define RESIDUAL -3
#define UNDEFINED -4

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
#define OPT_PARAM_NPLATES_RAW            0
#define OPT_PARAM_WIDTHPLATES_RAW        1
#define OPT_PARAM_HEIGHTPLATES_RAW       2
#define OPT_PARAM_MIN1CUT_RAW            3
#define OPT_PARAM_MAX1CUT_RAW            4
#define OPT_PARAM_MIN2CUT_RAW            5
#define OPT_PARAM_MINWASTE_RAW           6

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

extern unsigned int active_log; // Once log file is generated this var will be cleared.
extern ofstream log_file; // Reference to log file.

extern ofstream statistics_file; // Reference to statisticsLog file.
extern bool opened_file; // Boolean to test if one of needed files (batch, solution, defects and optimization parameters) was not opened.

extern string file_idx; // To save used file index (Normal mode).
extern string testPath; // To save path according to chosen test.
extern string instance; // To save used file index (Test mode).

extern int s_idx; // Solution index, used precise where to insert solution node in sol_items array.
extern int plates_nbr; // Save solution used plates number.
extern int stack_nbr; // Save batch stack number.
//extern int plates_list [PLATES_NBR_LIMIT][2];   // Save number of nodes for each plate used in user solution.
                                                  // plates_list[X][Y]:   X dimension contains plates Ids list
                                                  //                      Y dimension contains number of nodes of plate X
extern int lines_nbr; // csv file lines number.
extern int node_nbr; // Solution file nodes number.
extern int useful_node; // Solution useful nodes number (node not a branch or waste or residual).
extern int branch_node; // Solution branch nodes number
extern int residual_node; // Solution residual nodes number.
extern int waste_node; // Solution waste nodes number.
extern int batch_items; // Batch items number.
extern int constraint_error; // Success constraint, combination of solution's occured errors (refer to main.h to find errors list).
//extern int defects_nbr; // Defects file defects number.

extern int total_waste; // Sum of wasted area.
extern int total_useful; // Sum of used area.

extern int max_cut_stage; // Solution max used cut stage.

extern int plate_nbr_limit; // To save Optimization parameters plate number limit constraint.
extern int plate_w; // To save Optimization parameters plate's length constraint.
extern int plate_h; // To save Optimization parameters plate's width constraint.
extern int min1Cut; // To save Optimization parameters cut 1 & 2 min1Cut constraint.
extern int min2Cut; // To save Optimization parameters cut 1 & 2 min2Cut constraint.
extern int max1Cut; // To save Optimization parameters cut 1 & 2 max1Cut  constraint.
extern int waste_min; // To save Optimization parameters cut 1 & 2 minWasteWidth constraint.

extern int success;

