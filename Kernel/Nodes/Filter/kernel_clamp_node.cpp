#include "kernel_clamp_node.h"
namespace Kernel {
Clamp_Node::Clamp_Node(QObject *parent, NodeGraph *pNG) : Node(parent, pNG) {
    name = "钳制";
    color = QColor(Qt::GlobalColor::gray);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);

    AddParamPort(PortDataType::Float, "下界", true, 0);
    AddParamPort(PortDataType::Float, "上界", true, 150);

    //    AddNonPortParam(NonPortParamType::RangeFloat, "下界");
    //    AddNonPortParam(NonPortParamType::RangeFloat, "上界");
    //    nonPortParams[0]->InitData(0.0f, 0.0f, 256.0f);
    //    nonPortParams[1]->InitData(200.0f, 0.0f, 256.0f);

    AddOutputPort(PortDataType::Float2D, "输出高度场");
}

void Clamp_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    AddComputeShaderFromPath(":/ComputeShaders/Clamp.comp");
}

void Clamp_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    //    shaderPrograms[0]->setUniformValue("Low",
    //                                       nonPortParams[0]->data_rangefloat);
    //    shaderPrograms[0]->setUniformValue("High",
    //                                       nonPortParams[1]->data_rangefloat);
    shaderPrograms[0]->setUniformValue("Low", ParamPorts[0]->GetFloatData());
    shaderPrograms[0]->setUniformValue("High", ParamPorts[1]->GetFloatData());
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    unsigned int src = InputPorts[0]->GetBufferData();
    unsigned int data = OutputPorts[0]->GetBufferData();
    f.glBindImageTexture(0, src, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    f.glBindImageTexture(1, data, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void Clamp_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = data;
}

} // namespace Kernel
