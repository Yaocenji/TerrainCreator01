#ifndef USERINTERFACE_BEGINRESOLVEBUTTON_H
#define USERINTERFACE_BEGINRESOLVEBUTTON_H

#include <QBrush>
#include <QLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include "../Global/globalui.h"

/// 前置声明
namespace Render {

class Renderer;

}

namespace UserInterface {
/// 前置声明
class NodeEditorTerminal;

/// 开始地形计算按钮
class BeginResolveButton : public QPushButton {
    Q_OBJECT
protected:
    /// 按钮尺寸
    QPointF buttonSize;
    /// （用于显示定位用）渲染窗口器
    Render::Renderer *targetRenderer;
    /// 目标节点编辑器终端
    NodeEditorTerminal *targetEditor;
    /// 父的位置信息
    QRect parentRect;

public:
    /// 计时器
    QTimer *timer;
public slots:
    /// 更新位置，使之居中
    void TimeUpdate();

public:
    void paintEvent(QPaintEvent *event) override;
    //    void keyPressEvent(QKeyEvent *) override;
    virtual void DrawIcon(QPainter &p);

public slots:
    virtual void ClickedFunction();

public:
    explicit BeginResolveButton(Render::Renderer *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_BEGINRESOLVEBUTTON_H
