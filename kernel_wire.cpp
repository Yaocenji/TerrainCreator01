#include "kernel_wire.h"

namespace kernel {

Kernel_Wire::Kernel_Wire(QObject *parent, Port *ip, Port *op)
    : QObject(parent) {
    inputPort = nullptr;
    outputPort = nullptr;
    /// 检验接口类型
    if (ip->GetType() == PortType::Output &&
        (op->GetType() == PortType::Input ||
         op->GetType() == PortType::Param)) {
        inputPort = ip;
        outputPort = op;
    } else {
        qDebug() << "ERROR: port type is wrong in creating kernel wire!";
    }
}

} // namespace kernel
