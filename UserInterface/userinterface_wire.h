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
public:
    /// 目标kernel连线
    Kernel::Wire *targetWire;

public:
    /// 相连的接口
    Port *linkedPort[2];

public:
    void Draw(QPainter &p);
    /// 提供一个绘制类wire曲线的接口供外部对象调用
    static void DrawWire(QPainter &p, QPointF pos1, QPointF pos2);

public:
    explicit Wire(QObject *parent = nullptr, Kernel::Wire *tar = nullptr,
                  Port *lp1 = nullptr, Port *lp2 = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_WIRE_H
