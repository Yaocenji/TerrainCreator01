#include "kernel_terrace_node.h"
namespace Kernel {
Terrace_Node::Terrace_Node(QObject *parent, NodeGraph *pNG)
    : Node(parent, pNG) {
    name = "阶梯";
    color = QColor(Qt::GlobalColor::cyan);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);

    AddNonPortParam(NonPortParamType::RangeInt, "阶梯数");
    nonPortParams[0]->InitData(5, 1, 128);

    AddNonPortParam(NonPortParamType::RangeFloat, "阶梯锐利因数");
    nonPortParams[1]->InitData(0.5f, -1.0f, 1.0f);

    AddNonPortParam(NonPortParamType::Enum, "阶梯模式：单/双向");
    QVector<QString> enum_model;
    enum_model.push_back("单向/尖锐");
    enum_model.push_back("双向/平滑");
    nonPortParams[2]->InitData(0, enum_model);

    AddOutputPort(PortDataType::Float2D, "输出高度场");
}

void Terrace_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    AddComputeShaderFromPath(":/ComputeShaders/Terrace.comp");
}

void Terrace_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    shaderPrograms[0]->setUniformValue("TerraceNum",
                                       nonPortParams[0]->data_rangeint);
    shaderPrograms[0]->setUniformValue("Sharpness",
                                       nonPortParams[1]->data_rangefloat);
    shaderPrograms[0]->setUniformValue("Model", nonPortParams[2]->data_enum);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    unsigned int src = InputPorts[0]->GetBufferData();
    unsigned int data = OutputPorts[0]->GetBufferData();

    f.glBindImageTexture(0, src, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    f.glBindImageTexture(1, data, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void Terrace_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = data;
    globalinfo::ColorMap0 = 0;
}

} // namespace Kernel
