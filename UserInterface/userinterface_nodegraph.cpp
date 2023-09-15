#include "userinterface_nodegraph.h"

#include <type_traits>

namespace UserInterface {

NodeGraph::NodeGraph(QObject *parent, Kernel::NodeGraph *tarKernelGraph)
    : QObject(parent), targetKernelGraph(tarKernelGraph) {
    CameraUpLeft = QPointF(0.0, 0.0);
    CameraSizeControl = 0.5f;
    CameraSizeMagn = exp(CameraSizeControl);
    mState = UserInterface::MouseState::None;
    chosenNode.resize(0);
    chosenPort = nullptr;
    lockedNode = nullptr;
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

QVector<Node *> &NodeGraph::ChosenNode() {
    return chosenNode;
}

bool NodeGraph::ChooseOneNode(Node *tar) {
    bool flag = false;
    for (auto &i : Nodes) {
        if (tar == i) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        qDebug() << "错误：函数ChooseOneNode传入节点指针不在该节点图内";
        return false;
    } else {
        // 选择节点
        chosenNode.clear();
        chosenNode.push_back(tar);
        // 设置状态
        tar->Chosen();
        // 有无锁定显示的节点
        if (lockedNode != nullptr) {
            globalinfo::HeightFieldBuffer =
                lockedNode->targetNode->OutputPorts[0]->GetBufferData();
        }
        // 设置其他节点
        for (auto &i : Nodes) {
            if (i != tar) {
                i->state = NodeChosenState::None;
            }
        }
        return true;
    }
}

bool NodeGraph::ChooseASetOfNodes(QVector<Node *> tar) {
    bool allFlag = true;
    for (auto &i : tar) {
        bool flag = false;
        for (auto &j : Nodes) {
            if (i == j) {
                flag = true;
                break;
            }
        }
        if (flag)
            continue;
        else {
            allFlag = false;
            break;
        }
    }
    if (!allFlag) {
        qDebug() << "错误：函数ChooseOneNode传入节点指针不在该节点图内";
        return false;
    } else {
        chosenNode.clear();
        chosenNode.swap(tar);
        // 设置状态
        //        for (auto &i : chosenNode) {
        //            i->state = NodeChosenState::Chosen;
        //        }
        for (auto &j : Nodes) {
            if (chosenNode.contains(j)) {
                j->state = NodeChosenState::Chosen;
            } else {
                j->state = NodeChosenState::None;
            }
        }
        return true;
    }
}

bool NodeGraph::ChooseOneWire(Wire *tar) {
    bool flag = false;
    for (auto &i : Wires) {
        if (tar == i) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        qDebug() << "错误：函数ChooseOneWire传入wire指针不在该节点图内";
        return false;
    } else {
        // 选择节点
        chosenWire.clear();
        chosenWire.push_back(tar);
        // 设置状态
        tar->state = WireChosenState::Chosen;
        for (auto &i : Wires) {
            if (i != tar) {
                i->state = WireChosenState::None;
            }
        }
        return true;
    }
}

void NodeGraph::MoveChosenNodes(QPointF delta) {
    for (auto &i : chosenNode) {
        i->Move(delta);
    }
}

void NodeGraph::DeleteChosenNode() {
    for (auto &i : chosenNode) {
        DeleteNode(i);
    }
    chosenNode.clear();
    for (auto &i : Nodes) {
        i->state = NodeChosenState::None;
    }
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
        for (QVector<Wire *>::iterator it = Wires.begin(); it != Wires.end();) {
            // 如果对应的抽象指针不复存在
            if ((*it)->targetWire == nullptr) {
                Port *lastp1 = (*it)->linkedPort[0];
                Port *lastp2 = (*it)->linkedPort[1];
                delete (*it);
                it = Wires.erase(it);

                lastp1->PortLinkUpdateByWiresList(Wires);
                lastp2->PortLinkUpdateByWiresList(Wires);
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

bool NodeGraph::DeleteWire(Wire *tar) {
    if (tar == nullptr) {
        qDebug() << "警告：UI节点图删除ui::wire时，传入了空指针";
    }
    // 先在抽象层删除该wire
    if (tar->targetWire != nullptr) {
        targetKernelGraph->DeleteWire(tar->targetWire);
    } else {
        qDebug()
            << "警告：UI节点图删除ui::wire时，找不到与之相连的kernel::wire";
    }
    //  然后在交互层删除该wire
    bool flag = false;
    for (QVector<Wire *>::iterator it = Wires.begin(); it != Wires.end();
         it++) {
        // 找到要删除的wire
        if (*it == tar) {
            flag = true;
            Port *inport = tar->linkedPort[0];
            Port *outport = tar->linkedPort[1];

            // 释放内存
            // 标记为空指针
            delete tar;
            tar = nullptr;

            it = Wires.erase(it);
            inport->PortLinkUpdateByWiresList(Wires);
            outport->PortLinkUpdateByWiresList(Wires);
            break;
        }
    }
    qDebug() << "删除wire " << QString(flag ? "成功 " : "失败 ")
             << Wires.size();
    return flag;
}

bool NodeGraph::DeleteNode(Node *tar) {
    if (tar == nullptr) {
        qDebug() << "警告：UI节点图删除ui::node时，传入了空指针";
    }

    bool flag = false;
    for (QVector<Node *>::iterator it = Nodes.begin(); it != Nodes.end();
         it++) {
        // 找到要删除的wire
        if (*it == tar) {
            flag = true;

            // 先删除所有与之相连的Wire
            if (tar->targetNode != nullptr) {
                targetKernelGraph->DeleteNode(tar->targetNode);
            } else {
                qDebug() << "警告：UI节点图删除ui::"
                            "node时，找不到与之相连的kernel::node";
            }

            // 删除相关的wire
            QVector<Wire *> toBeDeletedWires = QVector<Wire *>();
            for (auto &i : Wires) {
                if (i->targetWire == nullptr) {
                    toBeDeletedWires.push_back(i);
                }
            }
            for (auto &i : toBeDeletedWires) {
                DeleteWire(i);
            }

            // 释放内存
            // 标记为空指针
            delete tar;
            tar = nullptr;

            it = Nodes.erase(it);
            break;
        }
    }
    return flag;
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

void NodeGraph::DrawWireCheckFrameBuffer(QPainter &p) {
    for (auto &i : Wires) {
        i->DrawFrameBuffer(p);
    }
}

bool NodeGraph::ClickDetectNodePort(QPointF &pos, Node *&clickedNode,
                                    Port *&clickedPort) {
    for (auto &i : Nodes) {
        if (i->ClickDetect(pos, clickedNode, clickedPort)) {
            return true;
        }
    }
    return false;
}

bool NodeGraph::ClickDetectWire(QPointF &pos, Wire *&clickedWire,
                                QPixmap &pfb) {
    for (auto &i : Wires) {
        if (i->ClickDetect(pos, clickedWire, pfb)) {
            return true;
        }
    }
    return false;
}

void NodeGraph::PortHoveredUpdate(Port *tar) {
    for (auto &i : Nodes) {
        i->PortHoveredUpdate(tar);
    }
}

void NodeGraph::WireHoveredUpdate(Wire *tar) {
    for (auto &i : Wires) {
        if (i->state != WireChosenState::Chosen) {
            if (i == tar) {
                i->state = WireChosenState::Hovered;
                qDebug() << "触发了wire悬浮检测";
            } else {
                i->state = WireChosenState::None;
            }
        }
    }
}

} // namespace UserInterface
