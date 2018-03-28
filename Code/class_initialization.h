#pragma once

#include "global_var.h"
#include <vector>

#include "glass_item.h"
#include "glass_plate.h"
#include "glass_node.h"

/// Create Plate Instance
void createPlate(string *token, int p);

/// Create Node Instance
void createNode(std::vector<GlassNode>& nodes, string *token, int n);

/// Create Item Instance
void createItem(std::vector<GlassItem>& items, string *token, int p);

