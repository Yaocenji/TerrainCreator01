#include "globalinfo.h"

globalinfo::globalinfo() {
}

int globalinfo::TerrainGrid = 2048;

float globalinfo::TerrainSize = 2048.0;

float globalinfo::TerrainHeight = 256.0;

bool globalinfo::useHeightFieldBuffer = false;

unsigned int globalinfo::ChosenHeightFieldBuffer = 0;
unsigned int globalinfo::ColorMap0 = 0;

bool globalinfo::keepingCalculating = false;

int RowVertsNumber() {
    return globalinfo::TerrainGrid + 2;
}

int RowRectsNumber() {
    return globalinfo::TerrainGrid + 1;
}
