#ifndef RENDERER_H
#define RENDERER_H

#include <QApplication>
#include <QDebug>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QPoint>
#include <QPointF>
#include <QTime>
#include <QTimer>
#include <QtMath>
#include <iostream>

#include "../Global/globalinfo.h"
#include "../Kernel/kernel_element_inc.h"
#include "Light/render_parallellight.h"
#include "render_camera.h"
#include "render_renderimage2d.h"
#include "render_rendertexture.h"
#include "render_terrainmesh.h"

namespace Render {
/// 程序的渲染窗口
class Renderer : public QOpenGLWidget, QOpenGLFunctions_4_5_Core {
    Q_OBJECT

protected:
    /// 为了流畅性，使用计时器
    QTimer *timer;
public slots:
    /// 计时器消息
    void TimerUpdate();

    /// 这一部分的变量在每次重新设置全局地形大小时会被重置
protected:
    /// 地形网格
    TerrainMesh *terrainMesh;
    /// （弃用）地形网格VBO
    QOpenGLBuffer *terrainVBO;
    /// （弃用）地形网格EBO
    QOpenGLBuffer *terrainEBO;
    /// （弃用）地形网格VAO
    QOpenGLVertexArrayObject *terrainVAO;
    /// 离屏渲染矩形网格VBO
    QOpenGLBuffer *screenVBO;
    /// 离屏交换矩形网格VAO
    QOpenGLVertexArrayObject *screenVAO;

protected:
    /// （弃用）地形平面顶点数据组
    float *panelVertices;
    /// （弃用）地形网格三角形顶点索引数据组
    unsigned int *panelIndices;
    /// 离屏渲染矩形网格顶点数据组
    float *rectVertices;

protected:
    /// 地形顶点着色器
    QOpenGLShader *terrainVert;
    /// 地形片元着色器——颜色
    QOpenGLShader *terrainFrag_DirectLight;
    /// 地形片元着色器——世界坐标
    QOpenGLShader *terrainFrag_WorldPos;
    /// 地形片元着色器——法线
    QOpenGLShader *terrainFrag_WorldNormal;

    /// 地形渲染着色器程序
    QOpenGLShaderProgram *terrainShaderProgram_DirectLight;
    /// 地形渲染着色器程序——世界空间坐标
    QOpenGLShaderProgram *terrainShaderProgram_WorldPos;
    /// 地形渲染着色器程序——世界空间法线
    QOpenGLShaderProgram *terrainShaderProgram_WorldNorm;
    /// 地形渲染着色器程序——基础色
    QOpenGLShaderProgram *terrainShaderProgram_Albedo;

    /// 光线追踪着色器
    QOpenGLShader *terrainShader_RayTracing;
    /// 光线追踪着色器程序
    QOpenGLShaderProgram *terrainShaderProgram_RayTracing;

    /// 离屏交换顶点着色器
    QOpenGLShader *swapVert;
    /// 离屏交换片元着色器
    QOpenGLShader *swapFrag;
    /// 离屏交换渲染着色器程序
    QOpenGLShaderProgram *swapShaderProgram;

protected:
    /// 缓冲RT——直接光照渲染
    RenderTexture *G_Buffer_DirectLight;
    /// 缓冲RT——世界空间坐标
    RenderTexture *G_Buffer_WorldPos;
    /// 缓冲RT——世界空间法线
    RenderTexture *G_Buffer_WorldNormal;
    /// 缓冲RT——基础色
    RenderTexture *G_Buffer_Albedo;

    /// 缓冲RI——光线追踪计算结果
    RenderImage2D *ANS_Buffer_RayTracing;

    /// （弃用）离屏渲染缓冲区frame buffer
    GLuint swapFrameBuffer;
    /// （弃用）离屏渲染缓冲区frame buffer的颜色缓冲和深度缓冲内容
    GLuint swapColorBuffer, swapDepthBuffer;

protected:
    /// 日光
    ParallelLight sunLight;
    /// 物体矩阵
    QMatrix4x4 model;
    /// 摄像机
    Camera *camera;
    /// （弃用）摄像机位置
    QVector3D camPos;
    /// （弃用）摄像机vp矩阵内容
    QMatrix4x4 view, proj;
    // （弃用）一系列摄像机参数
    float altitude;
    float azimuth;
    float distance;
    float scaleSensitive, rotateSensitive;

public:
    explicit Renderer(QWidget *parent = nullptr);

protected:
    /// QOpenGLWidget初始化函数
    void initializeGL() override;
    /// QOpenGLWidget窗口大小更改函数
    void resizeGL(int w, int h) override;
    /// QOpenGLWidget渲染绘制函数（包含渲染管线）
    void paintGL() override;

public:
    /// 地形全局设置改变函数
    void setTerrainInfo();

protected:
    /// 网格：顶点数据与三角形索引数据生成函数
    void generateMesh();
    /// 地形顶点数计数
    unsigned int verticiesCount() const;
    /// 地形顶点索引计数
    unsigned int indicesCount() const;
    /// 分配VBO/VAO/EBO资源
    void allocateBuffer();

    /// 准备离屏渲染资源，只在程序启动时调用一次
    void prepareSwapChain();
    /// （弃用）生成FBO/COLOR RENDERTEXTURE/DEPTH RENDERTEXTURE
    void createFrameBufferAndRenderTextures();
    /// （弃用）切换FBO的COLOR RENDERTEXTUREDEPTH RENDERTEXTURE
    void switchFrameBufferAndRenderTextures();

    /// 准备shader渲染资源，只在启动时调用
    void setRenderShaders();
    /// 着色器绑定与传参
    void bindShaderAndPassParam(QOpenGLShaderProgram &shader);

protected:
    /// （弃用）设置摄像机
    void setCameraInfo();
    /// （弃用）摄像机设置函数
    void setCameraInfo(int w, int h);

    /// 鼠标函数
    /// 是否正在控制
    bool isControlling;
    /// 摄像机控制模式
    CameraControlMode camCtrlMode;
    /// 上一时刻鼠标位置
    QPoint mouseOldPos;
    // 系列重载函数
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    /// 获取自身的上下文指针
    QOpenGLFunctions_4_5_Core *getFunctionAndContext();

signals:
};

} // namespace Render

#endif // RENDERER_H
