#ifndef USERINTERFACE_TITLEBAR_MAXIMIZEBUTTON_H
#define USERINTERFACE_TITLEBAR_MAXIMIZEBUTTON_H

#include <QObject>

#include "userinterface_titlebarbutton.h"

// 前置声明
class MainWindow;

namespace UserInterface {

// 前置声明
class TitleBar;

class TitleBarMaximizebutton : public TitleBarButton {
    Q_OBJECT

public:
    MainWindow *mainWindow;

public:
    void UpdatePos() override;
    void clickFunc() override;
    void Draw(QPainter &p) override;

public:
    explicit TitleBarMaximizebutton(QObject *parent = nullptr,
                                    TitleBar *titlebar = nullptr,
                                    MainWindow *mW = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_TITLEBAR_MAXIMIZEBUTTON_H
