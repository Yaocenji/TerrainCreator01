#ifndef USERINTERFACE_NODE_H
#define USERINTERFACE_NODE_H

#include <QColor>
#include <QDebug>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QVector2D>
#include <QVector>
#include <QWidget>

#include "../Global/globalui.h"
#include "../Kernel/kernel_node.h"
#include "userinterface_port.h"

namespace UserInterface {

/// 节点是否选中的状态
enum class NodeChosenState { None, Chosen };
/// 节点计算是否完成（可用）的状态
enum class NodeDataState { Unavailable, Available };

// 节点图前置声明
class NodeGraph;

class Node : public QObject {
    Q_OBJECT
public:
    /// 目标核节点
    Kernel::Node *targetNode;
    /// 所属节点图
    NodeGraph *parentNodeGraph;

public:
    /// 显示矩形信息
    QRectF rect;
    /// 颜色信息
    QColor color();
    /// 名字
    QString name;

public:
    /// 是否被锁定显示
    bool isLocked;
    /// 选中状态
    NodeChosenState state;
    /// 被选中时调用该函数
    void Chosen();
    // 接口数组
    // 分别是输入接口、输出接口和参数接口
    QVector<Port *> InputPorts;
    QVector<Port *> OutputPorts;
    QVector<Port *> ParamPorts;

public:
    /// 获取矩形信息
    QRectF GetRectInfo();
    /// 计算并设置节点矩形尺寸
    void CalSetRectSize();

    /// 绘制函数
    void Draw(QPainter &p);
    /// 点击检测
    bool ClickDetect(QPointF &pos, Node *&clickedNode, Port *&clickedPort);
    /// 移动至某个位置
    void Move(QPointF &pos);
    /// 接口状态更新1：悬浮
    void PortHoveredUpdate(Port *tar);

public:
    explicit Node(QObject *parent = nullptr, Kernel::Node *tar = nullptr,
                  NodeGraph *parentNG = nullptr, QPointF pos = QPointF(0, 0));

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODE_H
