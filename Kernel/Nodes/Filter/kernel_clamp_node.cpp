#include "kernel_clamp_node.h"
namespace Kernel {
Clamp_Node::Clamp_Node(QObject *parent, NodeGraph *pNG) : Node(parent, pNG) {
    name = "钳制";
    color = QColor(Qt::GlobalColor::gray);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);

    AddParamPort(PortDataType::Float, "下界", true);
    AddParamPort(PortDataType::Float, "上界", true);

    AddOutputPort(PortDataType::Float2D, "输出高度场");
}

void Clamp_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    AddComputeShaderFromPath(":/ComputeShaders/Clamp.comp");
}

void Clamp_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("Low", 100.0f);
    shaderPrograms[0]->setUniformValue("High", 200.0f);
    qDebug() << f.glGetError();

    unsigned int src = InputPorts[0]->GetBufferData();
    unsigned int data = OutputPorts[0]->GetBufferData();
    f.glBindImageTexture(0, src, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    f.glBindImageTexture(1, data, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    qDebug() << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    qDebug() << f.glGetError();
}

void Clamp_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = data;
}

} // namespace Kernel
