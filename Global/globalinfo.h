#ifndef GLOBALINFO_H
#define GLOBALINFO_H

#define DoNothing \
    ;             \
    ;             \
    ;

/// 创建和地形大小匹配的二维高度场
#define glCreateHeightField(glContext, data)                \
    {                                                       \
        glContext.glGenTextures(1, &data);                  \
        glContext.glBindTexture(GL_TEXTURE_2D, data);       \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_R32F, \
                                 globalinfo::TerrainGrid,   \
                                 globalinfo::TerrainGrid);  \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);          \
    }

/// 创建和地形大小匹配的二维RGBA32位浮点图像/向量数组
#define glCreateRGBAImage(glContext, data)                         \
    {                                                          \
        glContext.glGenTextures(1, &data);                     \
        glContext.glBindTexture(GL_TEXTURE_2D, data);          \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, \
                                 globalinfo::TerrainGrid,      \
                                 globalinfo::TerrainGrid);     \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);             \
    }

/// 创建任意大小的二维高度场/浮点数组
#define glCreateSizedHeightField(glContext, data, width, height)                     \
    {                                                                       \
        glContext.glGenTextures(1, &data);                                  \
        glContext.glBindTexture(GL_TEXTURE_2D, data);                       \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_R32F, width, height); \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);                          \
    }

/// 创建任意大小的二维RGBA32位浮点图像/向量数组
#define glCreateSizedRGBAImage(glContext, data, width, height)                 \
    {                                                                          \
        glContext.glGenTextures(1, &data);                                     \
        glContext.glBindTexture(GL_TEXTURE_2D, data);                          \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, width, height); \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);                             \
    }

/// 创建和地形大小匹配的纹理
#define glCreateTexture(glContext, data)                                 \
    {                                                                    \
        glContext.glGenTextures(1, &data);                               \
        glContext.glBindTexture(GL_TEXTURE_2D, data);                    \
        glContext.glTexImage2D(                                          \
            GL_TEXTURE_2D, 0, GL_RGB, globalinfo::TerrainGrid,           \
            globalinfo::TerrainGrid, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); \
        glContext.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  \
                                  GL_LINEAR);                            \
        glContext.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  \
                                  GL_LINEAR);                            \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);                       \
    }

#define glCreatePointCloud(glContext, data, size)                        \
    {                                                                    \
        glContext.glGenTextures(1, &data);                               \
        glContext.glBindTexture(GL_TEXTURE_2D, data);                    \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, size, 1); \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);                       \
    }

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
    /// 纹理通道1
    static unsigned int ColorMap0;

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
