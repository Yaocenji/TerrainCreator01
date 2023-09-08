#ifndef USERINTERFACE_NODEGRAPH_H
#define USERINTERFACE_NODEGRAPH_H

#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

// #include "../Kernel/kernel_nodegraph.h"
#include "Kernel/kernel_element_inc.h"
#include "userinterface_node.h"
#include "userinterface_port.h"
#include "userinterface_wire.h"

namespace UserInterface {

enum class MouseState {
    None,
    PressNode,
    PressPort,
    PressWire,
    CreateNode,
    Grab
};

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
    QVector<Node *> chosenNode;
    /// 当前选中的焦点接口
    Port *chosenPort;
    /// 当前选中的焦点wire
    QVector<Wire *> chosenWire;
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
    /// 获取当前选取的节点数组
    QVector<Node *> &ChosenNode();
    /// 单选一个节点
    bool ChooseOneNode(Node *tar);
    /// 选取一系列节点
    bool ChooseASetOfNodes(QVector<Node *> tar);
    /// 单选一个wire
    bool ChooseOneWire(Wire *tar);
    /// 移动选区的节点
    void MoveChosenNodes(QPointF delta);
    /// 获取当前选取的接口
    Port *&ChosenPort();

public:
    /// 绘制
    void Draw(QPainter &p);
    /// 点击检测：节点与接口
    bool ClickDetectNodePort(QPointF &pos, Node *&clickedNode,
                             Port *&clickedPort);
    /// 点击检测：连线
    bool ClickDetectWire(QPointF &pos, Wire *&clickedWire, QPixmap &pfb);
    /// 绘制wire的check-framebuffer
    void DrawWireCheckFrameBuffer(QPainter &p);
    /// 状态更新1：悬浮
    void PortHoveredUpdate(Port *tar);
    /// 状态更新2：连线悬浮
    void WireHoveredUpdate(Wire *tar);

public:
    /// 增删改查

    /// 添加函数——方法1
    template <typename T>
    bool addNode(QPointF pos);

    /**
     * @brief 添加函数——方法2
     * @param 参数1：位置
     * @param 参数2：构造函数指针
     * @return
     */
    bool addNodeByConstructor(QPointF pos, KernelNodeConstructorPtr ptr);

    /// 添加连线
    bool LinkWire(Port *lp1, Port *lp2);
    /// 清空全部连线
    void ClearWire();

    /// 删除连线
    bool DeleteWire(Wire *tar);

    /// 运行
    void RunNodeGraph(QOpenGLFunctions_4_5_Core &f);

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
