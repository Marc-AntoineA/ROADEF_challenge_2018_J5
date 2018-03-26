#ifndef PARSER_H
#define PARSER_H

#include "GlobalVar.h"

#include "classinitialization.h"
#include "builddata.h"

/// Optimization Parameters file Parser
void parseOptimizationParams (string path);

/// Batch file Parser
void parseBatch (string path);

/// Solution file Parser
void parseSolution (string path);

/// Defects file Parser
void parseDefects (string path);

#endif // PARSER_H
