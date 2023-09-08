#include "userinterface_titlebar_minimizbutton.h"

#include "./mainwindow.h"
#include "Global/globalfunc.h"
#include "userinterface_titlebar.h"

namespace UserInterface {

TitleBarMinimizButton::TitleBarMinimizButton(QObject *parent,
                                             TitleBar *titlebar, MainWindow *mW)
    : TitleBarButton(parent, titlebar), mainWindow(mW) {
}

void TitleBarMinimizButton::UpdatePos() {
    this->rect = QRect(titleBar->rect().width() - 3 * width, 0, width,
                       titleBar->rect().height());
}

void TitleBarMinimizButton::clickFunc() {
    mainWindow->showMinimized();
}

void TitleBarMinimizButton::Draw(QPainter &p) {
    QLine ln(rect.center() - QPoint(7, 0), rect.center() + QPoint(7, 0));
    QPen oldpen = p.pen();

    if (state == ButtonState::Hover) {
        p.setPen(QColor(0, 0, 0, 0));
        p.setBrush(QBrush(AverageColor(globalui::ui_background_color,
                                       globalui::ui_foreground_color)));
        p.drawRect(rect);
    } else if (state == ButtonState::Pressed) {
        p.setPen(QColor(0, 0, 0, 0));
        p.setBrush(QBrush(globalui::ui_foreground_color));
        p.drawRect(rect);
    }
    p.setBrush(QBrush(globalui::transparent_color));
    p.setPen(globalui::ui_icon_pen);

    p.drawLine(ln);

    p.setPen(oldpen);
}

} // namespace UserInterface
