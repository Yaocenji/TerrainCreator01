#ifndef USERINTERFACE_NODEEDITORTERMINAL_H
#define USERINTERFACE_NODEEDITORTERMINAL_H

#include <QBrush>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QWidget>

#include "../Global/userinterface_option.h"
#include "userinterface_node.h"
#include "userinterface_nodegraph.h"
#include "userinterface_port.h"
#include "userinterface_wire.h"

namespace UserInterface {

class NodeEditorTerminal : public QWidget {
    Q_OBJECT
protected:
    /// 要显示的目标图
    NodeGraph *targetNodeGraph;

protected:
    /// 为了流畅性，使用计时器
    QTimer *timer;
    /// 计时器重绘
    void TimerUpdate();

protected:
    /// 鼠标是否按下
    bool isPressing;
    /// 鼠标状态机
    MouseState mState;
    /// 鼠标最近一次点击的位置（屏幕空间）
    QPointF oldMousePos;
    /// 鼠标当前位置（世界空间）
    QPointF realMousePos;

public:
    /// 设置目标显示的节点图
    void SetTargetNodeGraph(NodeGraph *tar);

public:
    /// 绘制函数
    void paintEvent(QPaintEvent *event) override;
    /// 鼠标按下
    void mousePressEvent(QMouseEvent *event) override;
    /// 鼠标移动
    void mouseMoveEvent(QMouseEvent *event) override;
    /// 鼠标松开
    void mouseReleaseEvent(QMouseEvent *event) override;
    /// 滚轮事件
    void wheelEvent(QWheelEvent *event) override;

public:
    explicit NodeEditorTerminal(QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEEDITORTERMINAL_H
