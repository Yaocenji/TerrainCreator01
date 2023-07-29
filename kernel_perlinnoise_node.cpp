#include "kernel_perlinnoise_node.h"
namespace kernel {

Kernel_PerlinNoise_Node::Kernel_PerlinNoise_Node(QObject *parent)
    : Node(parent) {
    // 添加输出：一个二维高度场柏林噪声
    AddOutputPort(PortDataType::Float2D, "高度场输出");
    // 添加参数：种子
    AddParamPort(PortDataType::Float, "柏林噪声种子");
    // 添加参数：细节层次
    AddParamPort(PortDataType::Float, "柏林噪声倍频次数");
    // 添加参数：大小缩放
    AddParamPort(PortDataType::Float, "缩放倍率");
    // 添加参数：X轴偏移
    AddParamPort(PortDataType::Float, "X轴偏移");
    // 添加参数：Y轴偏移
    AddParamPort(PortDataType::Float, "Y轴偏移");
}

void Kernel_PerlinNoise_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    // 清空shader数组，重新准备
    shaderPrograms.clear();
    // 准备第一个shader：柏林噪声生成器
    QOpenGLShader *shader1; // = new QOpenGLShader(QOpenGLShader::Compute);
    // shader1->compileSourceFile(":/ComputeShaders/PerlinNoise.comp");
    QOpenGLShaderProgram *shaderProgram1; // = new QOpenGLShaderProgram();
                                          //    shaderProgram1->create();
    //    shaderProgram1->addShader(shader1);
    //    shaderProgram1->link();

    CreateComputeShaderFromPath(shader1, shaderProgram1,
                                ":/ComputeShaders/PerlinNoise.comp");

    this->shaderPrograms.push_back(shaderProgram1);
}

void Kernel_PerlinNoise_Node::Allocate(QOpenGLFunctions_4_5_Core &f) {
    // 准备缓存
    for (auto &i : OutputPorts) {
        i->AllocateBuffer(f);
    }
    for (auto &i : ParamPorts) {
        i->AllocateBuffer(f);
    }
}

void Kernel_PerlinNoise_Node::RunNode(QOpenGLFunctions_4_5_Core &f) {
    //    f.initializeOpenGLFunctions();
    qDebug() << f.glGetError();

    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    shaderPrograms[0]->setUniformValue("Detail", (int)5);
    shaderPrograms[0]->setUniformValue("Scale", 10.0f, 10.0f);
    shaderPrograms[0]->setUniformValue("Transform", 0.0f, 0.0f);
    qDebug() << f.glGetError();

    unsigned int data = OutputPorts[0]->GetBufferData();
    f.glBindImageTexture(0, data, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    qDebug() << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    qDebug() << f.glGetError();
}

void Kernel_PerlinNoise_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    //    f.initializeOpenGLFunctions();
    f.glBindImageTexture(0, data, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    globalinfo::useHeightFieldBuffer = true;
}

void Kernel_PerlinNoise_Node::Release(QOpenGLFunctions_4_5_Core &f) {
    // 释放所有缓存
    for (auto &i : OutputPorts) {
        i->DeleteBuffer(f);
    }
    for (auto &i : ParamPorts) {
        i->DeleteBuffer(f);
    }
}

unsigned int Kernel_PerlinNoise_Node::test(QOpenGLFunctions_4_5_Core &f) {
    QOpenGLShader *ts;
    QOpenGLShaderProgram *tsp;
    unsigned int test;
    f.glGenTextures(1, &test);
    f.glBindTexture(GL_TEXTURE_2D, test);
    f.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, globalinfo::TerrainGrid,
                     globalinfo::TerrainGrid);
    ts = new QOpenGLShader(QOpenGLShader::Compute);
    ts->compileSourceFile(":/ComputeShaders/PerlinNoise.comp");
    tsp = new QOpenGLShaderProgram();
    tsp->create();
    tsp->addShader(ts);
    tsp->link();

    tsp->bind();
    f.glBindImageTexture(0, test, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    tsp->setUniformValue("TerrainHeight", 256);
    tsp->setUniformValue("Detail", (int)5);
    tsp->setUniformValue("Scale", 1.0f, 1.0f);
    tsp->setUniformValue("Transform", 0.0f, 0.0f);
    f.glDispatchCompute(32, 32, 1);

    f.glBindImageTexture(0, test, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    return test;
}

} // namespace kernel
