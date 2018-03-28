#ifndef CHECKER_H
#define CHECKER_H

/*
 * Optimization Checker Algorithm 2017
 * This script is built to check user’s solution compatibility and validity.
 * This main script is composed of the following parts:
 *  - main() function called by default when program is executed.
 *  - Test Tode Tenu function.
 *  - StatisticsLog create function.
 *  - Display plate area usage function.
 *  - Constraints verification functions.
 *  - CSV files parse functions.
 *  - Tree structure build functions.
 *  - Classes instantiation functions.
**/

#include "global_var.h"

#include "parser.h"
#include "results.h"

#include "glass_item.h"
#include "glass_stack.h"
GlassItem* items;
GlassStack* stacks;

///Main Function
int checker();

/// Verify Identity & Sequence constraints
void verifyIdt_Sequence (void);

/// Verify Dimensions constraint
void verifyDimensions (void);

/// Verify Dimensions constraint of a node successors
void checkSuccDimensions (GlassNode parent);

/// Verify Defects overlap constraint
void verifyDefects (void);

/// Verify Item Production constraint
void verifyItemProduction (void);

#endif // CHECKER_H
