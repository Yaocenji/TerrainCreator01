#include "userinterface_titlebar.h"

#include "./Global/globalui.h"
#include "./mainwindow.h"
#include "Buttons/userinterface_titlebar_filebutton.h"
#include "userinterface_titlebar_maximizebutton.h"
#include "userinterface_titlebar_minimizbutton.h"
#include "userinterface_titlebar_quitbutton.h"
#include "userinterface_titlebarbutton.h"

namespace UserInterface {

TitleBar::TitleBar(QWidget *parent, MainWindow *mW)
    : QWidget(parent), mainWindow(mW) {
    isPressing = false;
    setMouseTracking(true);

    // 启动计时器
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(33);
    timer->start();
    // 根据计时器信号更新画面
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));

    // 添加叉按钮
    buttons.push_back(new TitleBarQuitButton(this, this));
    // 添加最大化/还原按钮
    buttons.push_back(new TitleBarMaximizebutton(this, this, mainWindow));
    // 添加最小化按钮
    buttons.push_back(new TitleBarMinimizButton(this, this, mainWindow));

    // 添加文件按钮
    buttons.push_back(new TitleBarFileButton(this, this));
}

void TitleBar::TimerUpdate() {
    static QRect myRect = this->rect();
    static unsigned int cnt = 0;
    // 计数器，用于在程序开始时总是更新，防止错误的初始化
    if (cnt <= 90) {
        cnt++;
        goto label;
    }
    // 每帧根据自身的rect和上一帧是否一致来判断是否需要通知子组件更新位置
    // 不更新
    if (myRect == this->rect()) {
        return;
    }
// 更新
label:;
    for (auto &i : buttons) {
        i->UpdatePos();
    }

    update();
    myRect = this->rect();
}

void TitleBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    // 反走样
    p.setRenderHint(QPainter::Antialiasing, true);

    p.setBrush(QBrush(globalui::ui_background_color));
    p.setPen(QPen(QColor(0, 0, 0, 0), 0));
    p.drawRect(this->rect());

    for (auto &i : buttons) {
        i->Draw(p);
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    isPressing = true;
    mousePressPos = event->pos();
    mouseOldPos = mousePressPos;
    mouseOldGlobalPos = event->globalPos();

    // 记录初次点击的对象
    pressedButton = nullptr;

    static QPointF posf = event->pos();
    posf = event->pos();

    // 鼠标点击事件判定
    for (auto &i : buttons) {
        if (UpdateButtonState(i, posf)) {
            pressedButton = i;
            break;
        }
    }
    update();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    static QPointF posf = event->pos();
    posf = event->pos();
    // 鼠标移动事件判定
    static bool flag = false;
    flag = false;
    for (auto &i : buttons) {
        flag = UpdateButtonState(i, posf);
        if (flag) break;
    }
    if (!flag) {
        if (isPressing && pressedButton == nullptr) {
            // 拖动窗口
            QPointF deltaPos = event->globalPos() - mouseOldGlobalPos;
            QPointF newpos = mainWindow->frameGeometry().topLeft() + deltaPos;
            mainWindow->move(newpos.x(), newpos.y());
            mouseOldGlobalPos = event->globalPos();
        }
    }
    update();
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event) {
    isPressing = false;
    mouseReleasePos = event->pos();
    // 鼠标松开事件判定
    static bool flag = false;
    flag = false;
    for (auto &i : buttons) {
        flag = UpdateButtonClick(i);
        if (flag) break;
    }
    update();
}

bool TitleBar::UpdateButtonState(TitleBarButton *button, QPointF pos) {
    if (button->ClickDetect(pos)) {
        if (!isPressing)
            button->state = ButtonState::Hover;
        else
            button->state = ButtonState::Pressed;
        for (auto &i : buttons) {
            if (i != button) i->state = ButtonState::None;
        }
        return true;
    } else {
        button->state = ButtonState::None;
        return false;
    }
}

bool TitleBar::UpdateButtonClick(TitleBarButton *button) {
    if (button->ClickDetect(mousePressPos) &&
        button->ClickDetect(mouseReleasePos)) {
        button->clickFunc();
        return true;
    }
    return false;
}

} // namespace UserInterface
