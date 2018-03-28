#ifndef PARSER_H
#define PARSER_H

#include "global_var.h"
#include "class_initialization.h"
#include "build_data.h"
#include "glass_item.h"
#include "glass_stack.h"

#include <vector>

/// Optimization Parameters file Parser
void parseOptimizationParams(string path);

/// Batch file Parser
std::vector<GlassStack> parseBatch(std::string path);

/// Solution file Parser
void parseSolution(string path);

/// Defects file Parser
void parseDefects(string path);

#endif // PARSER_H
