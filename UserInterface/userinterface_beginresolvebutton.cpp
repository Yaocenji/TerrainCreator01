#include "userinterface_beginresolvebutton.h"

#include "./Render/renderer.h"
#include "./UserInterface/userinterface_nodeeditorterminal.h"

namespace UserInterface {

BeginResolveButton::BeginResolveButton(Render::Renderer *parent)
    : QPushButton(parent), targetRenderer(parent) {
    buttonSize = QPointF(75, 50);

    parentRect = parent->rect();
    this->setFixedSize(buttonSize.x(), buttonSize.y());

    // 启动计时器
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(17);
    timer->start();
    // 根据计时器信号更新画面
    connect(timer, SIGNAL(timeout()), this, SLOT(TimeUpdate()));

    // 按钮触发函数连接槽
    connect(this, SIGNAL(clicked()), this, SLOT(ClickedFunction()));
}

void BeginResolveButton::TimeUpdate() {
    if (targetRenderer->rect() != parentRect) {
        QPointF pos = QPointF(
            targetRenderer->rect().center().x(),
            targetRenderer->rect().bottom() - size().height() / 2.0 - 8.0);
        QPointF target =
            pos - QPointF(size().width() / 2.0, size().height() / 2.0f);
        move(target.x(), target.y());
    }
}

void BeginResolveButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    // 反走样
    p.setRenderHint(QPainter::Antialiasing, true);

    p.setPen(QPen(globalui::ui_foreground_color, globalui::pen_width));
    p.setBrush(QBrush(globalui::ui_background_color));

    p.drawRoundedRect(rect().topLeft().x() + globalui::pen_width / 2.0,
                      rect().topLeft().y() + globalui::pen_width / 2.0,
                      rect().width() - globalui::pen_width,
                      rect().height() - globalui::pen_width,
                      globalui::ui_angle_radius, globalui::ui_angle_radius);
    // 绘制图标
    DrawIcon(p);
}

void BeginResolveButton::DrawIcon(QPainter &p) {
    static const QPointF icon[3] = {
        QPointF(rect().width() / 3.0, rect().height() / 4.0),
        QPointF(rect().width() / 3.0, rect().height() * 3.0 / 4.0),
        QPointF(rect().width() * 2.0 / 3.0, rect().height() / 2.0)};
    p.drawPolygon(icon, 3);
}

void BeginResolveButton::ClickedFunction() {
}

} // namespace UserInterface
