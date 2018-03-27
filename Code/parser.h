#ifndef PARSER_H
#define PARSER_H

#include "global_var.h"

#include "class_initialization.h"
#include "build_data.h"

/// Optimization Parameters file Parser
void parseOptimizationParams (string path);

/// Batch file Parser
void parseBatch (string path);

/// Solution file Parser
void parseSolution (string path);

/// Defects file Parser
void parseDefects (string path);

#endif // PARSER_H
