#ifndef RENDERER_H
#define RENDERER_H

#include <QDebug>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <iostream>

#include "globalinfo.h"
#include "kernel_perlinnoise_node.h"

/// 程序的渲染窗口
class Renderer : public QOpenGLWidget, QOpenGLFunctions_4_5_Core {
    Q_OBJECT
    /// 这一部分的变量在每次重新设置全局地形大小时会被重置
private:
    /// 地形网格VBO
    QOpenGLBuffer *terrainVBO;
    /// 地形网格EBO
    QOpenGLBuffer *terrainEBO;
    /// 地形网格VAO
    QOpenGLVertexArrayObject *terrainVAO;
    /// 离屏渲染矩形网格VBO
    QOpenGLBuffer *screenVBO;
    /// 离屏交换矩形网格VAO
    QOpenGLVertexArrayObject *screenVAO;

public:
    GLuint test;
    QOpenGLShader *ts;
    QOpenGLShaderProgram *tsp;

protected:
    /// 地形平面顶点数据组
    float *panelVertices;
    /// 地形网格三角形顶点索引数据组
    unsigned int *panelIndices;
    /// 离屏渲染矩形网格顶点数据组
    float *rectVertices;

private:
    /// 地形顶点着色器
    QOpenGLShader *terrainVert;
    /// 地形片元着色器
    QOpenGLShader *terrainFrag;
    /// 地形渲染着色器程序
    QOpenGLShaderProgram *terrainShaderProgram;
    /// 离屏交换顶点着色器
    QOpenGLShader *swapVert;
    /// 离屏交换片元着色器
    QOpenGLShader *swapFrag;
    /// 离屏交换渲染着色器程序
    QOpenGLShaderProgram *swapShaderProgram;

private:
    /// 离屏渲染缓冲区frame buffer
    GLuint swapFrameBuffer;
    /// 离屏渲染缓冲区frame buffer的颜色缓冲和深度缓冲内容
    GLuint swapColorBuffer, swapDepthBuffer;

private:
    /// 摄像机mvp矩阵内容
    QMatrix4x4 model, view, proj;

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

    /// 准备shader渲染资源，只在启动时调用
    void setRenderShaders();

protected:
    void setCameraInfo();

public:
    /// 获取自身的上下文指针
    QOpenGLFunctions_4_5_Core *getFunctionAndContext();

signals:
};

#endif // RENDERER_H
