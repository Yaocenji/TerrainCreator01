#ifndef GLOBALINFO_H
#define GLOBALINFO_H

/// 全局信息
class globalinfo {
public:
    /// 地形网格细分数
    static int TerrainGrid;
    /// 地形边长 单位：米
    static float TerrainSize;
    /// 地形高度最大值（最小值为0） 单位：米
    static float TerrainHeight;
    /// 渲染选项：是否使用高度场数据
    static bool useHeightFieldBuffer;

public:
    globalinfo();
};

#endif // GLOBALINFO_H