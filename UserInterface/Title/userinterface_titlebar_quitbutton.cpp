#include "userinterface_titlebar_quitbutton.h"

#include "Global/globalfunc.h"
#include "userinterface_titlebar.h"

namespace UserInterface {

TitleBarQuitButton::TitleBarQuitButton(QObject *parent, TitleBar *titlebar)
    : TitleBarButton(parent, titlebar) {
}

void TitleBarQuitButton::UpdatePos() {
    this->rect = QRect(titleBar->rect().width() - width, 0, width,
                       titleBar->rect().height());
}

void TitleBarQuitButton::clickFunc() {
    QApplication *app;
    app->quit();
}

void TitleBarQuitButton::Draw(QPainter &p) {
    QLineF cross1(rect.center() - QPoint(7, 7), rect.center() + QPoint(7, 7));
    QLineF cross2(rect.center() - QPoint(-7, 7), rect.center() + QPoint(-7, 7));

    QPen oldpen = p.pen();

    if (state == ButtonState::Hover) {
        p.setPen(QColor(0, 0, 0, 0));
        p.setBrush(QBrush(
            AverageColor(globalui::ui_background_color, QColor(166, 25, 0))));
        p.drawRect(rect);
    } else if (state == ButtonState::Pressed) {
        p.setPen(QColor(0, 0, 0, 0));
        p.setBrush(QBrush(QColor(166, 25, 0)));
        p.drawRect(rect);
    }
    p.setPen(globalui::ui_icon_pen);

    p.drawLine(cross1);
    p.drawLine(cross2);

    p.setPen(oldpen);
}

} // namespace UserInterface
