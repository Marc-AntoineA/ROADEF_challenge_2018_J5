#pragma once

#include <vector>

#include "global_var.h"
#include "parser.h"
#include "glass_item.h"
#include "glass_stack.h"
#include "glass_node.h"



///Main Function
int checker();

/**
 * This function verifies the Identity and sequence constraints
 * Identity:
 *      this function verifies that all solution items are identical to batch items
 *      with the possibility to invert width and height.
 * Sequence:
 *      this function verifies that user solution respects the production sequence
 *      given in the batch file.
 * WARNING: stacks is changed (the current_idx)
**/
bool verifyIdt_Sequence(std::vector<GlassStack>& stacks, const std::vector<GlassNode>& solution);


/**
 * This function verifies dimensions constraint for first node cut level.
 * It verifies nodes superposing, cut stage, if nodes dimensions respect prefixed optimization parameters
 * if node's x, y, width and height are coherent with parent node (dependent on cut stage) and do not overflow it's area,
 * and verifies that nodes widths sum is equal to parent area.
**/
bool verifyDimensions(const std::vector<GlassPlate>& plates, const std::vector<GlassNode>& solution);

/**
 * This function is recursive, it verifies dimensions constraint of node successors.
**/
void checkSuccDimensions(GlassNode parent);

/**
 * This function verifies if solution items overlap defect(s)
 * given in defects csv file.
**/
bool verifyDefects(const std::vector<GlassPlate>& plates, const std::vector<GlassNode>& solution);

/**
 * This function verifies if all batch items are produced,
 * not produced and/or duplicated.
**/
bool verifyItemProduction(const std::vector<GlassStack>& stacks, const std::vector<GlassNode>& solution);
