#include "kernel_simpleoverlay_node.h"
namespace Kernel {
SimpleOverlay_Node::SimpleOverlay_Node(QObject *parent, NodeGraph *pNG)
    : Node(parent, pNG) {
    name = "简单纹理叠加渲染";
    color = QColor(Qt::GlobalColor::yellow);

    AddInputPort(PortDataType::Float2D, "输入高度场", false);
    AddInputPort(PortDataType::RGBA2D, "输入纹理", false);
}

void SimpleOverlay_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
}

void SimpleOverlay_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    DoNothing;
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void SimpleOverlay_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int heightField = InputPorts[0]->GetBufferData();
    unsigned int Texture = InputPorts[1]->GetBufferData();

    globalinfo::ChosenHeightFieldBuffer = heightField;
    globalinfo::ColorMap0 = Texture;
}

} // namespace Kernel
