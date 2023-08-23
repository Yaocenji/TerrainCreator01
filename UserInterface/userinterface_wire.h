#ifndef USERINTERFACE_WIRE_H
#define USERINTERFACE_WIRE_H

#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QString>

#include "../Kernel/kernel_wire.h"

namespace UserInterface {

/// 前置声明
class Port;

class Wire : public QObject {
    Q_OBJECT
protected:
    /// 目标kernel连线
    Kernel::Wire *targetWire;
    /// 相连的接口
    Port *linkedPort[2];

public:
    void Draw(QPainter &p);

public:
    explicit Wire(QObject *parent = nullptr, Kernel::Wire *tar = nullptr,
                  Port *lp1 = nullptr, Port *lp2 = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_WIRE_H
