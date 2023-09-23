#include "kernel_perlinnoise_node.h"
namespace Kernel {

PerlinNoise_Node::PerlinNoise_Node(QObject *parent, NodeGraph *pNG)
    : Node(parent, pNG) {
    name = "柏林噪声";
    color = QColor(124, 198, 255);
    // 添加输出：一个二维高度场柏林噪声
    AddOutputPort(PortDataType::Float2D, "高度场输出");

    // 0添加参数：种子
    AddParamPort(PortDataType::Float, "噪声种子", true, 1.0);
    // 1添加参数：大小缩放
    AddParamPort(PortDataType::Float, "缩放倍率", true, 1.0);
    ParamPorts[1]->isRanged = true;
    ParamPorts[1]->rangefloat_l = 0.01f;
    ParamPorts[1]->rangefloat_r = 5.0f;
    // 2添加参数：X轴偏移
    AddParamPort(PortDataType::Float, "X轴偏移", true, 0.0);
    ParamPorts[2]->isRanged = true;
    ParamPorts[2]->rangefloat_l = -10.0f;
    ParamPorts[2]->rangefloat_r = 10.0f;

    // 3添加参数：Y轴偏移
    AddParamPort(PortDataType::Float, "Y轴偏移", true, 0.0);
    ParamPorts[3]->isRanged = true;
    ParamPorts[3]->rangefloat_l = -10.0f;
    ParamPorts[3]->rangefloat_r = 10.0f;

    // 3添加参数：高度偏移
    AddParamPort(PortDataType::Float, "高度偏移", true, -0.25);
    ParamPorts[4]->isRanged = true;
    ParamPorts[4]->rangefloat_l = -2.0f;
    ParamPorts[4]->rangefloat_r = 2.0f;

    // 3添加参数：高度对比
    AddParamPort(PortDataType::Float, "高度对比", true, 2.0f);
    ParamPorts[5]->isRanged = true;
    ParamPorts[5]->rangefloat_l = 0.0f;
    ParamPorts[5]->rangefloat_r = 10.0f;

    // 添加参数：细节层次
    AddNonPortParam(NonPortParamType::RangeInt, "噪声细节");
    nonPortParams[0]->InitData(10, 1, 15);
}

void PerlinNoise_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    // 为什么要在init函数中，每次运行的时候重新创建shader？
    // --因为每次计算的时候，shader可能会出现宏开关的变化，因此需要重新特化

    // 清空shader数组，重新准备
    ClearShaders();
    // 准备第一个shader：柏林噪声生成器
    AddComputeShaderFromPath(":/ComputeShaders/PerlinNoise.comp");
}

// void PerlinNoise_Node::Allocate(QOpenGLFunctions_4_5_Core &f) {
// }

void PerlinNoise_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    shaderPrograms[0]->setUniformValue("TerrainSize", globalinfo::TerrainSize);

    shaderPrograms[0]->setUniformValue("Seed", ParamPorts[0]->GetFloatData());
    shaderPrograms[0]->setUniformValue("Detail",
                                       nonPortParams[0]->data_rangeint);
    shaderPrograms[0]->setUniformValue("Scale", ParamPorts[1]->GetFloatData(),
                                       ParamPorts[1]->GetFloatData());
    shaderPrograms[0]->setUniformValue("Transform",
                                       ParamPorts[2]->GetFloatData(),
                                       ParamPorts[3]->GetFloatData());
    shaderPrograms[0]->setUniformValue("HeightOffset",
                                       ParamPorts[4]->GetFloatData());
    shaderPrograms[0]->setUniformValue("HeightContrast",
                                       ParamPorts[5]->GetFloatData());
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    unsigned int data = OutputPorts[0]->GetBufferData();
    f.glBindImageTexture(0, data, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void PerlinNoise_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = data;
    globalinfo::ColorMap0 = 0;
}

unsigned int PerlinNoise_Node::test(QOpenGLFunctions_4_5_Core &f) {
    QOpenGLShader *ts;
    QOpenGLShaderProgram *tsp;
    unsigned int test;
    f.glGenTextures(1, &test);
    f.glBindTexture(GL_TEXTURE_2D, test);
    f.glTexStorage2D(GL_TEXTURE_2D, 8, GL_R32F, globalinfo::TerrainGrid,
                     globalinfo::TerrainGrid);
    ts = new QOpenGLShader(QOpenGLShader::Compute);
    ts->compileSourceFile(":/ComputeShaders/PerlinNoise.comp");
    tsp = new QOpenGLShaderProgram();
    tsp->create();
    tsp->addShader(ts);
    tsp->link();

    tsp->bind();
    f.glBindImageTexture(0, test, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    tsp->setUniformValue("TerrainHeight", 256);
    tsp->setUniformValue("Detail", (int)5);
    tsp->setUniformValue("Scale", 1.0f, 1.0f);
    tsp->setUniformValue("Transform", 0.0f, 0.0f);
    f.glDispatchCompute(32, 32, 1);

    f.glBindImageTexture(0, test, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

    return test;
}

} // namespace Kernel
