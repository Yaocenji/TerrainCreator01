#ifndef KERNEL_WIRE_H
#define KERNEL_WIRE_H

#include <QDebug>
#include <QObject>

namespace kernel {

class Port;
/// 连线
class Wire : public QObject {
    Q_OBJECT
private:
    /// 输入的节点
    Port *inputPort;
    /// 输出的节点
    Port *outputPort;

public:
    Port *GetInput();
    Port *GetOutput();

public:
    explicit Wire(QObject *parent = nullptr, Port *ip = nullptr,
                  Port *op = nullptr);

signals:
};

} // namespace kernel
#endif // KERNEL_WIRE_H
