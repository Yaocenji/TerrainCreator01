#ifndef USERINTERFACE_NODEGRAPH_H
#define USERINTERFACE_NODEGRAPH_H

#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

#include "../Kernel/kernel_nodegraph.h"
#include "userinterface_node.h"
#include "userinterface_port.h"
#include "userinterface_wire.h"

namespace UserInterface {

enum class MouseState { None, PressNode, PressPort, CreateNode, Grab };

class NodeGraph : public QObject {
    Q_OBJECT
protected:
    /// 要显示的目标节点图
    Kernel::NodeGraph *targetKernelGraph;

    /// 摄像机左上坐标
    QPointF CameraUpLeft;
    /// 摄像机缩放倍率
    float CameraSizeMagn;
    /// 摄像机缩放倍率的控制
    float CameraSizeControl;
    /// 鼠标状态机
    MouseState mState;
    /// 当前选中的焦点节点
    Node *chosenNode;
    /// 当前选中的焦点接口
    Port *chosenPort;
    /// 所有的节点组成的vector
    QVector<Node *> Nodes;
    /// 所有连线组成的vector
    QVector<Wire *> Wires;
    /// 鼠标最近一次点击的位置
    QPointF oldMousePos;

public:
    /// 获取摄像机位置
    QPointF &GetCamPos();
    /// 摄像机位置变动
    void CamPosMove(QPointF delta);
    /// 获取摄像机缩放
    float &GetCamSize();
    /// 获取缩放倍率控制数（自然对数
    float &GetCamSizeCtrl();
    /// （弃用）读写鼠标状态机
    MouseState &MouseState();

public:
    /// 获取节点数组
    QVector<Node *> &GetNodes();
    /// 获取当前选取的节点
    Node *&ChosenNode();
    Port *&ChosenPort();

public:
    /// 绘制
    void Draw(QPainter &p);
    /// 点击检测
    bool ClickDetect(QPointF &pos, Node *&clickedNode, Port *&clickedPort);
    /// 状态更新1：悬浮
    void PortSuspensionUpdate(Port *tar);

public:
    /// 增删改查
    /// 添加函数——方法1
    template <typename T>
    bool addNode(QPointF pos);

    /// 添加连线
    bool LinkWire(Port *lp1, Port *lp2);

public:
    explicit NodeGraph(QObject *parent = nullptr,
                       Kernel::NodeGraph *tarKernelGraph = nullptr);

signals:
};

template <typename T>
bool NodeGraph::addNode(QPointF pos) {
    // 首先判断传入的类是否是抽象Node基类的派生类
    if (!std::is_base_of<Kernel::Node, T>::value) {
        qDebug() << "ERROR：创建的节点时，传入的类型并非是节点类";
        return false;
    } else {
        // 创建新的kernel节点
        T *newKernelNode = new T(targetKernelGraph, targetKernelGraph);
        // 在kernelgraph中添加新的kernel节点
        targetKernelGraph->AddNode(newKernelNode);
        // 创建对应的UI节点并添加之
        Node *newUINode = new Node(this, newKernelNode, this, pos);
        this->Nodes.push_back(newUINode);
    }
}

} // namespace UserInterface
#endif // USERINTERFACE_NODEGRAPH_H
