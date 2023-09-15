#include "globalinfo.h"

globalinfo::globalinfo() {
}

// int globalinfo::TerrainGrid = 1024;
int globalinfo::TerrainGrid = 2048;

float globalinfo::TerrainSize = 1024.0;

float globalinfo::TerrainHeight = 256.0;

bool globalinfo::useHeightFieldBuffer = false;

unsigned int globalinfo::HeightFieldBuffer = 0;

bool globalinfo::keepingCalculating = false;

/// 预设纹理通道
unsigned int globalinfo::ColorTexture_01 = 0;

unsigned int globalinfo::NormalMap = 0;

int RowVertsNumber() {
    return globalinfo::TerrainGrid + 2;
}

int RowRectsNumber() {
    return globalinfo::TerrainGrid + 1;
}
