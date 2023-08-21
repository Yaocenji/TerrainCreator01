#include "kernel_nodegraph.h"

#include "kernel_clamp_node.h"
#include "kernel_node.h"
#include "kernel_perlinnoise_node.h"
#include "kernel_wire.h"

namespace Kernel {

NodeGraph::NodeGraph(QObject *parent, bool iSg, Node *oN)
    : QObject(parent), isSubgraph(iSg), parentNode(oN) {
    isFinished = false;
}

void NodeGraph::AddNode(Node *n) {
    nodes.push_back(n);
    return;
}

Wire *NodeGraph::LinkWire(Port *a, Port *b) {
    Port *in = nullptr;
    Port *out = nullptr;
    // 进行类型检查
    if ((a->GetType() == PortType::Input || a->GetType() == PortType::Param) &&
        b->GetType() == PortType::Output) {
        in = b;
        out = a;
    }
    // 反之
    else if ((b->GetType() == PortType::Input ||
              b->GetType() == PortType::Param) &&
             a->GetType() == PortType::Output) {
        in = a;
        out = b;
    } else {
        qDebug() << "ERROR: The type of ports to be linked are not conforming.";
        return nullptr;
    }
    // 进行冲突检查
    // 需要检查是否已经有连线了
    for (QVector<Wire *>::iterator it = wires.begin(); it != wires.end();) {
        // 如果已经有以out为输出接口的连线了
        // 把所有以out为输出接口的连线全部删除，并且对于涉及到的接口更新一遍信息
        if ((*it)->GetOutput() == out) {
            Port *another = (*it)->GetInput();
            it = wires.erase(it);
            another->UpdateLinkInfo(wires);
            // out->UpdateLinkInfo(wires);
        } else {
            it++;
        }
    }
    // 创建并加入
    Wire *newWire = new Wire(this, in, out);
    wires.push_back(newWire);
    // 对涉及到的接口更新信息
    in->UpdateLinkInfo(wires);
    out->UpdateLinkInfo(wires);
    return newWire;
}

Node *NodeGraph::GetParentMacroNode() {
    if (isSubgraph) {
        return parentNode;
    } else {
        return nullptr;
    }
}

bool NodeGraph::RunNodeGraph(QOpenGLFunctions_4_5_Core &f) {
    // 如果是子图的话，那么从输出节点开始递归
    if (isSubgraph) {
        // TODO 从输出节点开始递归计算
    }
    for (int i = 0; i < nodes.length(); i++) {
        if (!(nodes[i]->isFinished)) {
            nodes[i]->InitGL(f);
            nodes[i]->Allocate(f);
            nodes[i]->RunNode(f);
            nodes[i]->Choose(f);
        }
    }
    return true;
}

} // namespace kernel
