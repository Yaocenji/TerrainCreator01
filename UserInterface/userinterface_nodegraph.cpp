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

void NodeGraph::Draw(QPainter &p) {
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
