#ifndef USERINTERFACE_NODEEDITORTERMINAL_H
#define USERINTERFACE_NODEEDITORTERMINAL_H

#include <QBrush>
#include <QDropEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QWidget>

#include "../Global/globalui.h"
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
public slots:
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
    /// 运行节点图
    void RunNodeGraph(QOpenGLFunctions_4_5_Core &f);

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
    /// 拖拽进入事件
    void dragEnterEvent(QDragEnterEvent *event) override;
    /// 拖拽移动事件
    void dragMoveEvent(QDragMoveEvent *event) override;
    /// 拖拽释放事件
    void dropEvent(QDropEvent *event) override;

public:
    explicit NodeEditorTerminal(QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEEDITORTERMINAL_H
