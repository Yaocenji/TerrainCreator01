#ifndef KERNEL_WIRE_H
#define KERNEL_WIRE_H

#include <QObject>

#include "kernel_port.h"

namespace kernel {

/// 连线
class Kernel_Wire : public QObject {
    Q_OBJECT
private:
    /// 输入的节点
    Port *inputPort;
    /// 输出的节点
    Port *outputPort;

public:
public:
    explicit Kernel_Wire(QObject *parent = nullptr, Port *ip = nullptr,
                         Port *op = nullptr);

signals:
};

} // namespace kernel
#endif // KERNEL_WIRE_H
