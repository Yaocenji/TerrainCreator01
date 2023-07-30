#include "kernel_node.h"
namespace kernel {

Node::Node(QObject *parent, NodeGraph *pNM)
    : QObject(parent), parentNodeManager(pNM) {
}

NodeGraph *Node::GetParentNodeManager() {
    return parentNodeManager;
}

void Node::AddInputPort(PortDataType dt, QString n) {
    Port *npb = new Port(this, this, PortType::Input, dt, n);
    this->InputPorts.push_back(npb);
}

void Node::AddOutputPort(PortDataType dt, QString n) {
    Port *npb = new Port(this, this, PortType::Output, dt, n);
    this->OutputPorts.push_back(npb);
}

void Node::AddParamPort(PortDataType dt, QString n) {
    Port *npb = new Port(this, this, PortType::Param, dt, n);
    this->ParamPorts.push_back(npb);
}

} // namespace kernel
