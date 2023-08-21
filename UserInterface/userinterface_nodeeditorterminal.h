#ifndef USERINTERFACE_NODEEDITORTERMINAL_H
#define USERINTERFACE_NODEEDITORTERMINAL_H

#include <QBrush>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
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
    /// 鼠标状态机
    MouseState mState;
    /// 鼠标最近一次点击的位置
    QPointF oldMousePos;

public:
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

public:
    explicit NodeEditorTerminal(QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEEDITORTERMINAL_H
