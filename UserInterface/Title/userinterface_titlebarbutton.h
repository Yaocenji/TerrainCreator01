#ifndef USERINTERFACE_TITLEBARBUTTON_H
#define USERINTERFACE_TITLEBARBUTTON_H

#include <QBrush>
#include <QDebug>
#include <QLine>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QRect>
#include <QTimer>

#include "./Global/globalui.h"

namespace UserInterface {

/// 前置声明
class TitleBar;

enum class ButtonState {
    /// 无
    None,
    /// 悬停
    Hover,
    /// 按下
    Pressed
};

class TitleBarButton : public QObject {
    Q_OBJECT

public slots:
    /// 更新位置函数
    virtual void UpdatePos();

protected:
    /// 所属的titlebar
    TitleBar *titleBar;
    /// 位置信息
    QRect rect;

public:
    /// 按钮的宽度
    float width;
    /// 按钮状态
    ButtonState state;

public:
    /// 绘制
    virtual void Draw(QPainter &p);
    /// 鼠标是否在范围内检测
    bool ClickDetect(QPointF &pos);
    /// 单击触发函数
    virtual void clickFunc();

public:
    explicit TitleBarButton(QObject *parent = nullptr,
                            TitleBar *titlebar = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_TITLEBARBUTTON_H
