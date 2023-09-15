#include "kernel_number_node.h"
namespace Kernel {
Number_Node::Number_Node(QObject *parent, NodeGraph *pNG) : Node(parent, pNG) {
    name = "数字";
    color = QColor(Qt::GlobalColor::darkGreen);

    AddNonPortParam(NonPortParamType::RangeFloat, "数字");
    nonPortParams[0]->InitData(1.5f, 0.01f, 9.99f);

    AddOutputPort(PortDataType::Float, "输出数字");
}

void Number_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    //    ClearShaders();
    //    AddComputeShaderFromPath(":/ComputeShaders/Invert.comp");
}

void Number_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    //    shaderPrograms[0]->bind();
    //    shaderPrograms[0]->setUniformValue("TerrainHeight",
    //                                       globalinfo::TerrainHeight);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    //    unsigned int src = InputPorts[0]->GetBufferData();
    //    unsigned int data = OutputPorts[0]->GetBufferData();

    //    f.glBindImageTexture(0, src, 0, GL_FALSE, 0, GL_READ_WRITE,
    //    GL_R32F); f.glBindImageTexture(1, data, 0, GL_FALSE, 0,
    //    GL_READ_WRITE, GL_R32F);
    OutputPorts[0]->SetData(nonPortParams[0]->data_rangefloat);
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    //    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
    //                        globalinfo::TerrainGrid / 32, 1);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void Number_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    DoNothing
}

} // namespace Kernel
