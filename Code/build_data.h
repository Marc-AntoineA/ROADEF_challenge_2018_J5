#ifndef BUILDDATA_H
#define BUILDDATA_H

#include "global_var.h"

/// Data Structure Builder
void buildDataStructure (GlassNode *node);

/// Data Structure Builder
void checkSuccessors (GlassNode *node, GlassNode *sol_items, int plate_id, GlassNode &parent, int c_end, int index);

#endif // BUILDDATA_H
