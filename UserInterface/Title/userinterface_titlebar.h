#ifndef USERINTERFACE_TITLEBAR_H
#define USERINTERFACE_TITLEBAR_H

#include <QApplication>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>
#include <QWidget>

// 前置声明
class MainWindow;

namespace UserInterface {

// 前置声明

class TitleBarButton;

class TitleBar : public QWidget {
    Q_OBJECT

protected:
    /// 计时器
    QTimer *timer;

    /// 按钮数组
    QVector<TitleBarButton *> buttons;
    /// 鼠标是否按下
    bool isPressing;
    /// 鼠标按下位置
    QPointF mousePressPos;
    /// 鼠标抬起位置
    QPointF mouseReleasePos;
    /// 鼠标上一帧的位置
    QPointF mouseOldPos;
    /// 鼠标上一帧的全局位置
    QPointF mouseOldGlobalPos;
    /// 鼠标按下时点到的按钮
    TitleBarButton *pressedButton;

public:
    MainWindow *mainWindow;

public slots:
    void TimerUpdate();

public:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool UpdateButtonState(TitleBarButton *button, QPointF pos);
    bool UpdateButtonClick(TitleBarButton *button);

public:
    explicit TitleBar(QWidget *parent = nullptr, MainWindow *mW = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_TITLEBAR_H
