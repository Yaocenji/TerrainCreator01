#include "userinterface_nodegraph.h"

#include <type_traits>

namespace UserInterface {

NodeGraph::NodeGraph(QObject *parent, Kernel::NodeGraph *tarKernelGraph)
    : QObject(parent), targetKernelGraph(tarKernelGraph) {
    CameraUpLeft = QPointF(0.0, 0.0);
    CameraSizeMagn = 1.0f;
    CameraSizeControl = 0.0f;
    mState = UserInterface::MouseState::None;
    chosenNode = nullptr;
    chosenPort = nullptr;
    oldMousePos = QPointF(0.0, 0.0);
}

QPointF &NodeGraph::GetCamPos() {
    return CameraUpLeft;
}

float &NodeGraph::GetCamSize() {
    return CameraSizeMagn;
}

void NodeGraph::CamPosMove(QPointF delta) {
    CameraUpLeft += delta;
}

float &NodeGraph::GetCamSizeCtrl() {
    return CameraSizeControl;
}

MouseState &NodeGraph::MouseState() {
    return mState;
}

QVector<Node *> &NodeGraph::GetNodes() {
    return Nodes;
}

Node *&NodeGraph::ChosenNode() {
    return chosenNode;
}

Port *&NodeGraph::ChosenPort() {
    return chosenPort;
}

bool NodeGraph::LinkWire(Port *lp1, Port *lp2) {
    // 试图在抽象节点图中添加连线
    Kernel::Wire *ansWire = targetKernelGraph->LinkWire(
        lp1->GetTargetKernelPort(), lp2->GetTargetKernelPort());
    // 判断是否添加成功
    if (ansWire != nullptr) {
        // TODO 改进：应该在每次连接后检查相关节点的连线是否依然存在
        Wire *newWire = new Wire(this, ansWire, lp1, lp2);
        Wires.push_back(newWire);
        lp1->Link();
        lp2->Link();
        return true;
    } else {
        return false;
    }
}

void NodeGraph::Draw(QPainter &p) {
    for (auto &i : Wires) {
        i->Draw(p);
    }
    for (auto &i : Nodes) {
        i->Draw(p);
    }
}

bool NodeGraph::ClickDetect(QPointF &pos, Node *&clickedNode,
                            Port *&clickedPort) {
    for (auto &i : Nodes) {
        if (i->ClickDetect(pos, clickedNode, clickedPort)) {
            return true;
        }
    }
    return false;
}

void NodeGraph::PortSuspensionUpdate(Port *tar) {
    for (auto &i : Nodes) {
        i->PortSuspensionUpdate(tar);
    }
}

} // namespace UserInterface
