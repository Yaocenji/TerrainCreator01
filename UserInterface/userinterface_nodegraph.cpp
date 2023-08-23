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

QPointF NodeGraph::GetCamPos() {
    return CameraUpLeft;
}

float NodeGraph::GetCamSize() {
    return CameraSizeMagn;
}

void NodeGraph::CamPosMove(QPointF delta) {
    CameraUpLeft += delta;
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

bool NodeGraph::LinkWire(Port *lp1, Port *lp2) {
    // 试图在抽象节点图中添加连线
    Kernel::Wire *ansWire = targetKernelGraph->LinkWire(
        lp1->GetTargetKernelPort(), lp2->GetTargetKernelPort());
    // 判断是否添加成功
    if (ansWire != nullptr) {
        Wire *newWire = new Wire(this, ansWire, lp1, lp2);
        Wires.push_back(newWire);
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

} // namespace UserInterface
