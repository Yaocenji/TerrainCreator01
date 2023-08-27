#include "userinterface_titlebar_maximizebutton.h"

#include "./mainwindow.h"
#include "userinterface_titlebar.h"
namespace UserInterface {

TitleBarMaximizebutton::TitleBarMaximizebutton(QObject *parent,
                                               TitleBar *titlebar,
                                               MainWindow *mW)
    : TitleBarButton(parent, titlebar), mainWindow(mW) {
}

void TitleBarMaximizebutton::UpdatePos() {
    this->rect = QRect(titleBar->rect().width() - 2 * width, 0, width,
                       titleBar->rect().height());
}

void TitleBarMaximizebutton::clickFunc() {
    static bool isMaxed = false;
    static QRect mainWindowRect = mainWindow->rect();
    if (isMaxed) {
        mainWindow->setWindowState(Qt::WindowNoState);
        mainWindow->resize(mainWindowRect.size());
        mainWindow->move(mainWindowRect.topLeft());
        isMaxed = false;
    } else {
        mainWindowRect = mainWindow->geometry();
        //        mainWindow->showMaximized();

        mainWindow->setWindowState(Qt::WindowMaximized);
        isMaxed = true;
    }
}

void TitleBarMaximizebutton::Draw(QPainter &p) {
    QRect rc(rect.center() - QPoint(7, 6), QSize(14, 12));

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

    p.setBrush(QBrush(globalui::transparent_color));
    p.setPen(globalui::ui_icon_pen);

    p.drawRect(rc);

    p.setPen(oldpen);
}

} // namespace UserInterface
