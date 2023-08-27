#include "userinterface_titlebarbutton.h"

#include "userinterface_titlebar.h"

namespace UserInterface {

TitleBarButton::TitleBarButton(QObject *parent, TitleBar *titlebar)
    : QObject(parent), titleBar(titlebar) {
    // 初始化状态
    state = ButtonState::None;
    width = 60;
    UpdatePos();
}

void TitleBarButton::UpdatePos() {
    //    this->rect = QRect(titleBar->rect().width() - width, 0, width,
    //                       titleBar->rect().height());
}

void TitleBarButton::Draw(QPainter &p) {
    //    static const QLineF cross1(rect.left() + 1.0 / 3.0 * rect.height(),
    //                               rect.top() + 1.0 / 3.0 * rect.height(),
    //                               rect.left() + 2.0 / 3.0 * rect.height(),
    //                               rect.top() + 2.0 / 3.0 * rect.height());
    //    static const QLineF cross2(rect.left() + 2.0 / 3.0 * rect.height(),
    //                               rect.top() + 1.0 / 3.0 * rect.height(),
    //                               rect.left() + 1.0 / 3.0 * rect.height(),
    //                               rect.top() + 2.0 / 3.0 * rect.height());

    //    QPen oldpen = p.pen();

    //    if (state == ButtonState::Hover) {
    //        p.setPen(QColor(0, 0, 0, 0));
    //        p.setBrush(QBrush(QColor(166, 25, 0)));
    //        p.drawRect(rect);
    //    }
    //    p.setPen(globalui::ui_icon_pen);

    //    p.drawLine(cross1);
    //    p.drawLine(cross2);

    //    p.setPen(oldpen);
}

bool TitleBarButton::ClickDetect(QPointF &pos) {
    if (rect.contains(pos.x(), pos.y())) {
        //        qDebug() << "detect";
        return true;
    } else {
        return false;
    }
}

void TitleBarButton::clickFunc() {
}

} // namespace UserInterface
