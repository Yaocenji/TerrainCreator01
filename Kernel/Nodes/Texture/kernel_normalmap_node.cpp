#include "kernel_normalmap_node.h"
namespace Kernel {
NormalMap_Node::NormalMap_Node(QObject *parent, NodeGraph *pNG)
    : Node(parent, pNG) {
    name = "法线纹理";
    color = QColor(Qt::GlobalColor::darkBlue);

    AddOutputPort(PortDataType::RGBA2D, "输出纹理");
}

void NormalMap_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    // TODO shader
    AddComputeShaderFromPath(":/ComputeShaders/NormalMap.comp");
}

void NormalMap_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    shaderPrograms[0]->setUniformValue("TerrainGrid", globalinfo::TerrainGrid);
    shaderPrograms[0]->setUniformValue("TerrainSize", globalinfo::TerrainSize);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    unsigned int src = InputPorts[0]->GetBufferData();
    unsigned int ansMap = OutputPorts[0]->GetBufferData();

    f.glBindImageTexture(0, src, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    f.glBindImageTexture(1, ansMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    f.glDispatchCompute(globalinfo::TerrainGrid / 32,
                        globalinfo::TerrainGrid / 32, 1);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void NormalMap_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::HeightFieldBuffer = 0;
    globalinfo::ColorTexture_01 = data;
}

} // namespace Kernel
