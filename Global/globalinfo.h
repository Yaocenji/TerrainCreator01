#ifndef GLOBALINFO_H
#define GLOBALINFO_H

#define DoNothing \
    ;             \
    ;             \
    ;

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
    /// 渲染选项：当前使用的高度场
    static unsigned int ChosenHeightFieldBuffer;

    /// 解算选项：是否持续自动解算
    static bool keepingCalculating;

public:
    globalinfo();
};

/// 一行的顶点数
int RowVertsNumber();
/// 一行的矩形数
int RowRectsNumber();

#endif // GLOBALINFO_H
