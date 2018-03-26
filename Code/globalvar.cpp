#include "GlobalVar.h"

GlassPlate *plate;
GlassNode *sol_items;
GlassStack *stacks;
GlassItem *items;

unsigned int active_log=1;
ofstream log_file;
ofstream statistics_file;
bool opened_file = true;

string file_idx;
string testPath;
string instance;

unsigned int plates_list [PLATES_NBR_LIMIT][2];

unsigned int s_idx = 0;
unsigned int plates_nbr = 0;
unsigned int stack_nbr = 0;
unsigned int lines_nbr = 0;
unsigned int node_nbr = 0;
unsigned int useful_node = 0;
unsigned int branch_node = 0;
unsigned int residual_node = 0;
unsigned int waste_node = 0;
unsigned int batch_items = 0;
unsigned int constraint_error = 0;
unsigned int defects_nbr = 0;

unsigned int total_waste = 0;
unsigned int total_useful = 0;

int max_cut_stage = 0;

unsigned int plate_nbr_limit = 0;
unsigned int plate_w = 0;
unsigned int plate_h = 0;
unsigned int min1Cut = 0;
unsigned int min2Cut = 0;
unsigned int max1Cut = 0;
unsigned int waste_min = 0;

unsigned int success = 1;
