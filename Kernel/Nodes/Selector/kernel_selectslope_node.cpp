#include "kernel_selectslope_node.h"
namespace Kernel {
SelectSlope_Node::SelectSlope_Node(QObject *parent, NodeGraph *pNG)
    : Node(parent, pNG) {
    name = "选择坡度";
    color = QColor(Qt::GlobalColor::yellow);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);
    AddOutputPort(PortDataType::RGBA2D, "输出纹理");

    AddNonPortParam(NonPortParamType::RangeFloat, "最大角度");
    nonPortParams[0]->InitData(80.0f, 0.0f, 90.0f);

    AddNonPortParam(NonPortParamType::RangeFloat, "最小角度");
    nonPortParams[1]->InitData(20.0f, 0.0f, 90.0f);

    AddNonPortParam(NonPortParamType::Enum, "过渡模式");
    QVector<QString> strs;
    strs.push_back("默认");
    strs.push_back("向外");
    strs.push_back("向内");
    nonPortParams[2]->InitData(0, strs);

    AddNonPortParam(NonPortParamType::RangeFloat, "过渡强度");
    nonPortParams[3]->InitData(10.0f, 0.0f, 90.0f);
}

void SelectSlope_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    ClearShaders();
    // TODO shader
    AddComputeShaderFromPath(":/ComputeShaders/SelectSlope.comp");
}

void SelectSlope_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    shaderPrograms[0]->bind();
    shaderPrograms[0]->setUniformValue("TerrainHeight",
                                       globalinfo::TerrainHeight);
    shaderPrograms[0]->setUniformValue("TerrainGrid", globalinfo::TerrainGrid);
    shaderPrograms[0]->setUniformValue("TerrainSize", globalinfo::TerrainSize);

    shaderPrograms[0]->setUniformValue("max_angle",
                                       nonPortParams[0]->data_rangefloat);
    shaderPrograms[0]->setUniformValue("min_angle",
                                       nonPortParams[1]->data_rangefloat);
    shaderPrograms[0]->setUniformValue("smooth_model",
                                       nonPortParams[2]->data_enum);
    shaderPrograms[0]->setUniformValue("smooth_radius_angle",
                                       nonPortParams[3]->data_rangefloat);

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

void SelectSlope_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = 0;
    globalinfo::ColorMap0 = data;
}

} // namespace Kernel
