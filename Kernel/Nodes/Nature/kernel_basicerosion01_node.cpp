#include "kernel_basicerosion01_node.h"

#include <QRandomGenerator>

namespace Kernel {
BasicErosion01_Node::BasicErosion01_Node(QObject *parent, NodeGraph *pNG)
    : Node(parent, pNG) {
    name = "基本侵蚀 1";
    color = QColor(Qt::GlobalColor::cyan);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);

    AddOutputPort(PortDataType::Float2D, "输出高度场");

    AddOutputPort(PortDataType::Float2D, "地表径流量统计");

    AddNonPortParam(NonPortParamType::RangeInt, "迭代次数");
    nonPortParams[0]->InitData(12000, 1, 50000);

    AddNonPortParam(NonPortParamType::RangeInt, "地表径流最大流动长度");
    nonPortParams[1]->InitData(600, 100, 1500);

    AddNonPortParam(NonPortParamType::RangeFloat, "地表流体惯性");
    nonPortParams[2]->InitData(0.8f, 0.001f, 0.999f);

    AddNonPortParam(NonPortParamType::RangeFloat, "地表粘性（最小坡度系数）");
    nonPortParams[3]->InitData(0.001f, 0.0f, 0.9f);

    AddNonPortParam(NonPortParamType::RangeFloat, "地表径流固体沉积物携带上限");
    nonPortParams[4]->InitData(2.0f, 0.5f, 12.0f);

    AddNonPortParam(NonPortParamType::RangeFloat, "沉积系数");
    nonPortParams[5]->InitData(0.45f, 0.001f, 0.999f);

    AddNonPortParam(NonPortParamType::RangeFloat, "侵蚀系数");
    nonPortParams[6]->InitData(0.1f, 0.001f, 0.999f);

    AddNonPortParam(NonPortParamType::RangeFloat, "重力加速度");
    nonPortParams[7]->InitData(1.0f, 0.01f, 10.0f);

    AddNonPortParam(NonPortParamType::RangeFloat, "地表径流蒸发强度");
    nonPortParams[8]->InitData(0.08f, 0.001f, 0.5f);

    AddNonPortParam(NonPortParamType::RangeInt, "侵蚀半径");
    nonPortParams[9]->InitData(5, 1, 40);

    AddNonPortParam(NonPortParamType::RangeFloat, "地表硬度系数");
    nonPortParams[10]->InitData(0.12f, 0.0f, 1.0f);

    AddNonPortParam(NonPortParamType::RangeFloat, "地表径流初速度");
    nonPortParams[11]->InitData(2.42f, 0.0f, 8.0f);

    AddNonPortParam(NonPortParamType::RangeInt, "流动检测半径");
    nonPortParams[12]->InitData(3, 0, 15);
}

void BasicErosion01_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    // 添加着色器：复制
    AddComputeShaderFromPath(":/ComputeShaders/CopyHeightField.comp");
    // 添加着色器：基础侵蚀01
    AddComputeShaderFromPath(":/ComputeShaders/BasicErosion01.comp");
    // 添加着色器：基础侵蚀01
    AddComputeShaderFromPath(":/ComputeShaders/TestErosion.comp");
}

void BasicErosion01_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    unsigned int src = InputPorts[0]->GetBufferData();
    unsigned int data0 = OutputPorts[0]->GetBufferData();
    unsigned int data1 = OutputPorts[1]->GetBufferData();

    // 第一步：先复制一份buffer
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    f.glBindImageTexture(0, src, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    f.glBindImageTexture(1, data0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    DEBUG_GL << name << "节点计算时：gl错误验证2" << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    DEBUG_GL << name << "节点计算时：gl错误验证3" << f.glGetError();

    // 侵蚀启动
    // 因为SetImage2D函数会破坏着色器绑定，因此先计算之
    // 生成一张线性减小核buffer
    unsigned int linearKernel;
    // 最大半径40，对应变长81，但是81小于OpenGL纹理最小边长，所以使用128
    glCreateSizedHeightField(f, linearKernel, 128, 128);

    int radius = nonPortParams[9]->data_rangeint;
    int kernelSize = (radius * 2 + 1);
    float *kernel = new float[kernelSize * kernelSize];
    float sum = 0;
    float maxDist = radius * sqrt(2.0f);
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            float dist =
                sqrt(pow(float(i - radius), 2.0) + pow(float(j - radius), 2.0));

            kernel[i * kernelSize + j] = 1.0 - dist / maxDist;

            sum += kernel[i * kernelSize + j];
        }
    }
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i * kernelSize + j] /= sum;
        }
    }

    SetImage2D(f, linearKernel, kernel, kernelSize * kernelSize,
               radius * 2 + 1);

    delete[] kernel;

    // 生成一张雨滴初始位置buffer
    // TODO
    //    unsigned int dropInitialPos;
    //    // 纹理大小128*128，支持16384液滴模拟并行运算
    //    glCreateImage2D(f, dropInitialPos, 128, 128);

    //    QVector2D *dropInitialPosData = new QVector2D[256];

    //    for (int i = 0; i < 256; ++i) {
    //        dropInitialPosData[i].setX(
    //            QRandomGenerator::global()->generateDouble());
    //        dropInitialPosData[i].setY(
    //            QRandomGenerator::global()->generateDouble());
    //    }

    //    SetImage2D(f, dropInitialPos, dropInitialPosData, 256, 128);

    //    delete[] dropInitialPosData;

    DEBUG_GL << name << "节点计算时：gl错误验证4" << f.glGetError();

    // 绑定侵蚀shader

    shaderPrograms[1]->bind();

    shaderPrograms[1]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    shaderPrograms[1]->setUniformValue("TerrainGrid", globalinfo::TerrainGrid);
    shaderPrograms[1]->setUniformValue("TerrainSize", globalinfo::TerrainSize);

    shaderPrograms[1]->setUniformValue("p_max_water_len",
                                       nonPortParams[1]->data_rangeint);
    shaderPrograms[1]->setUniformValue("p_inertia",
                                       nonPortParams[2]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_minSlope",
                                       nonPortParams[3]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_capacity",
                                       nonPortParams[4]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_deposition",
                                       nonPortParams[5]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_erosion",
                                       nonPortParams[6]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_gravity",
                                       nonPortParams[7]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_evaporation",
                                       nonPortParams[8]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_radius",
                                       nonPortParams[9]->data_rangeint);
    shaderPrograms[1]->setUniformValue("p_hardness",
                                       nonPortParams[10]->data_rangefloat);
    shaderPrograms[1]->setUniformValue("p_speed",
                                       nonPortParams[11]->data_rangefloat);

    shaderPrograms[1]->setUniformValue("test", nonPortParams[12]->data_int);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    f.glBindImageTexture(0, data0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    f.glBindImageTexture(1, data1, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    f.glBindImageTexture(2, linearKernel, 0, GL_FALSE, 0, GL_READ_WRITE,
                         GL_R32F);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    for (int i = 0; i < nonPortParams[0]->data_rangeint; i++) {
        // 随机雨滴位置序列
        QVector2D initialPos[256];
        for (auto &i : initialPos) {
            i.setX(QRandomGenerator::global()->generateDouble());
            i.setY(QRandomGenerator::global()->generateDouble());
        }
        shaderPrograms[1]->setUniformValueArray("InitialDropPos", initialPos,
                                                256);
        f.glDispatchCompute(256 / 32, 1, 1);
    }

    f.glDeleteTextures(1, &linearKernel);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void BasicErosion01_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data0 = OutputPorts[0]->GetBufferData();
    unsigned int data1 = OutputPorts[1]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = data0;
    globalinfo::ColorMap0 = 0;
    qDebug() << "侵蚀节点，选择了" << data0
             << globalinfo::ChosenHeightFieldBuffer;
}

} // namespace Kernel
