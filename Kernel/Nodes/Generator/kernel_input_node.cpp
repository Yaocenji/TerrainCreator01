#include "kernel_input_node.h"

#include "../../kernel_nodegraph.h"
namespace Kernel {

Input_Node::Input_Node(QObject *parent) : Node(parent) {
}

void Input_Node::InitGL(QOpenGLFunctions_4_5_Core &f) {
    DoNothing
}

void Input_Node::Allocate(QOpenGLFunctions_4_5_Core &f) {
    DoNothing
}

void Input_Node::CalculateNode(QOpenGLFunctions_4_5_Core &f) {
    Node *parentMacroNode = parentNodeGraph->GetParentMacroNode();
    for (int i = 0; i <= this->OutputPorts.length(); i++) {
        OutputPorts[i]->CopyFrom(f, parentMacroNode->InputPorts[i]);
        OutputPorts[i]->isAvailable = true;
    }
}

void Input_Node::Choose(QOpenGLFunctions_4_5_Core &f) {
    DoNothing
}

void Input_Node::Release(QOpenGLFunctions_4_5_Core &f) {
    DoNothing
}

} // namespace Kernel
