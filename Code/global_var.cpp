#include "global_var.h"

int active_log=1;
ofstream log_file;
ofstream statistics_file;
bool opened_file = true;

string file_idx;
string testPath;
string instance;

int plates_list [PLATES_NBR_LIMIT][2];

int s_idx = 0;
//int plates_nbr = 0;
int stack_nbr = 0;
int lines_nbr = 0;
int node_nbr = 0;
int useful_node = 0;
int branch_node = 0;
int residual_node = 0;
int waste_node = 0;
int batch_items = 0;
int constraint_error = 0;
//int defects_nbr = 0;

int total_waste = 0;
int total_useful = 0;

int max_cut_stage = 0;

int plate_nbr_limit = 0;
int plate_w = 6000;
int plate_h = 3210;
int min1Cut = 100;
int min2Cut = 100;
int max1Cut = 3500;
int waste_min = 20;

int success = 1;
