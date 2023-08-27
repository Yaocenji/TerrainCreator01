#ifndef USERINTERFACE_TITLEBAR_FILEBUTTON_H
#define USERINTERFACE_TITLEBAR_FILEBUTTON_H

#include <QObject>

#include "../userinterface_titlebarbutton.h"

namespace UserInterface {

// 前置声明
class TitleBar;

class TitleBarFileButton : public TitleBarButton {
    Q_OBJECT

public:
    void UpdatePos() override;
    void clickFunc() override;
    void Draw(QPainter &p) override;

public:
    explicit TitleBarFileButton(QObject *parent = nullptr,
                                TitleBar *titlebar = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_TITLEBAR_FILEBUTTON_H
