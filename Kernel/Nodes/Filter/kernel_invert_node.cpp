#include "kernel_invert_node.h"
namespace Kernel {
Invert_Node::Invert_Node(QObject *parent, NodeGraph *pNG) : Node(parent, pNG) {
    name = "反转";
    color = QColor(Qt::GlobalColor::gray);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);

    AddOutputPort(PortDataType::Float2D, "输出高度场");
}

void Invert_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    AddComputeShaderFromPath(":/ComputeShaders/Invert.comp");
}

void Invert_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
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

void Invert_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::HeightFieldBuffer = data;
    globalinfo::ColorTexture_01 = 0;
}

} // namespace Kernel
