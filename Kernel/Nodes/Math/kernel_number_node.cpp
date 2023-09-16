#include "kernel_number_node.h"
namespace Kernel {
Number_Node::Number_Node(QObject *parent, NodeGraph *pNG) : Node(parent, pNG) {
    name = "数字";
    color = QColor(Qt::GlobalColor::green);

    AddNonPortParam(NonPortParamType::RangeFloat, "数字");
    nonPortParams[0]->InitData(1.5f, 0.01f, 9.99f);

    AddOutputPort(PortDataType::Float, "输出数字");
}

void Number_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    DoNothing
}

void Number_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();

    OutputPorts[0]->SetData(nonPortParams[0]->data_rangefloat);

    DEBUG_GL << name << "节点计算时：gl错误验证1" << f.glGetError();
}

void Number_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    unsigned int data = OutputPorts[0]->GetBufferData();
    globalinfo::ChosenHeightFieldBuffer = data;
}

} // namespace Kernel
