#ifndef USERINTERFACE_TITLEBAR_QUITBUTTON_H
#define USERINTERFACE_TITLEBAR_QUITBUTTON_H

#include <QWidget>

#include "userinterface_titlebarbutton.h"

namespace UserInterface {

// 前置声明
class TitleBar;

class TitleBarQuitButton : public TitleBarButton {
    Q_OBJECT

public:
    void UpdatePos() override;
    void clickFunc() override;
    void Draw(QPainter &p) override;

public:
    explicit TitleBarQuitButton(QObject *parent = nullptr,
                                TitleBar *titlebar = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_TITLEBAR_QUITBUTTON_H
