#ifndef CLASSINITIALIZATION_H
#define CLASSINITIALIZATION_H

#include "GlobalVar.h"

/// Create Plate Instance
void createPlate (string *token, int p);

/// Create Node Instance
void createNode (GlassNode *node, string *token, int p);

/// Create Item Instance
void createItem (GlassItem *items, string *token, int p);

#endif // CLASSINITIALIZATION_H
