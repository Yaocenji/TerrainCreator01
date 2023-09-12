#include "globalinfo.h"

globalinfo::globalinfo() {
}

// int globalinfo::TerrainGrid = 1024;
int globalinfo::TerrainGrid = 3072;

float globalinfo::TerrainSize = 1024.0;

float globalinfo::TerrainHeight = 256.0;

bool globalinfo::useHeightFieldBuffer = false;

unsigned int globalinfo::ChosenHeightFieldBuffer = 0;

bool globalinfo::keepingCalculating = true;
