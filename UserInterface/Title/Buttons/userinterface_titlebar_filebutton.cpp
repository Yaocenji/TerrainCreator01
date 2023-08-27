#include "userinterface_titlebar_filebutton.h"

#include "../userinterface_titlebar.h"

namespace UserInterface {

TitleBarFileButton::TitleBarFileButton(QObject *parent, TitleBar *titlebar)
    : TitleBarButton(parent, titlebar) {
}

void TitleBarFileButton::UpdatePos() {
    this->rect = QRect(0, 0, width, titleBar->rect().height());
}

void TitleBarFileButton::clickFunc() {
}

void TitleBarFileButton::Draw(QPainter &p) {
    QPen oldpen = p.pen();

    if (state == ButtonState::Hover) {
        p.setPen(QColor(0, 0, 0, 0));
        p.setBrush(QBrush(globalui::AverageColor(
            globalui::ui_background_color, globalui::ui_foreground_color)));
        p.drawRect(rect);
    } else if (state == ButtonState::Pressed) {
        p.setPen(QColor(0, 0, 0, 0));
        p.setBrush(QBrush(globalui::ui_foreground_color));
        p.drawRect(rect);
    }

    p.setPen(globalui::ui_icon_pen);

    p.setFont(globalui::ui_font);
    p.drawText(rect.center() + QPoint(-14, 8), "文件");

    p.setPen(oldpen);
}

} // namespace UserInterface
