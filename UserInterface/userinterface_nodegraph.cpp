#include "userinterface_nodegraph.h"

#include <type_traits>

namespace UserInterface {

NodeGraph::NodeGraph(QObject *parent, Kernel::NodeGraph *tarKernelGraph)
    : QObject(parent), targetKernelGraph(tarKernelGraph) {
    CameraUpLeft = QPointF(0.0, 0.0);
    CameraSizeControl = 0.5f;
    CameraSizeMagn = exp(CameraSizeControl);
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

bool NodeGraph::addNodeByConstructor(QPointF pos,
                                     KernelNodeConstructorPtr ptr) {
    // 创建新的kernel节点
    Kernel::Node *newKernelNode = (*ptr)(targetKernelGraph, targetKernelGraph);
    // 在kernelgraph中添加新的kernel节点
    targetKernelGraph->AddNode(newKernelNode);
    // 创建对应的UI节点并添加之
    Node *newUINode = new Node(this, newKernelNode, this, pos);
    this->Nodes.push_back(newUINode);

    return true;
}

bool NodeGraph::LinkWire(Port *lp1, Port *lp2) {
    // 试图在抽象节点图中添加连线
    Kernel::Wire *ansWire = targetKernelGraph->LinkWire(
        lp1->GetTargetKernelPort(), lp2->GetTargetKernelPort());
    qDebug() << targetKernelGraph->wires.size();
    // 判断是否添加成功
    if (ansWire != nullptr) {
        Wire *newWire = new Wire(this, ansWire, lp1, lp2);
        Wires.push_back(newWire);
        lp1->Link();
        lp2->Link();

        //        改进：应该在每次连接后检查相关节点的连线是否依然存在
        // 重新从抽象wire中同步
        //        ClearWire();
        //        for (auto &i : targetKernelGraph->wires) {
        //            Wire *newWire = new Wire(this, i,
        //            i->GetInput()->targetUIPort,
        //                                     i->GetInput()->targetUIPort);
        //            Wires.push_back(newWire);
        //            i->GetInput()->targetUIPort->Link();
        //            i->GetInput()->targetUIPort->Link();
        //        }

        for (QVector<Wire *>::iterator it = Wires.begin(); it != Wires.end();) {
            // 如果对应的抽象指针不复存在
            if ((*it)->targetWire == nullptr) {
                Port *lastp1 = (*it)->linkedPort[0];
                Port *lastp2 = (*it)->linkedPort[1];
                delete (*it);
                lastp1->PortLinkUpdateByWiresList(Wires);
                lastp2->PortLinkUpdateByWiresList(Wires);
                it = Wires.erase(it);
                if (it == Wires.end()) break;
            } else {
                it++;
            }
        }
        return true;
    } else {
        return false;
    }
}

void NodeGraph::ClearWire() {
    for (int i = 0; i < Wires.size(); i++) {
        Wires[i]->linkedPort[0]->Unlink();
        Wires[i]->linkedPort[1]->Unlink();
        delete Wires[i];
    }
    Wires.clear();
}

void NodeGraph::RunNodeGraph(QOpenGLFunctions_4_5_Core &f) {
    targetKernelGraph->RunNodeGraph(f);
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
