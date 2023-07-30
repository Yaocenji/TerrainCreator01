#include "kernel_wire.h"

#include "kernel_port.h"

namespace kernel {

Wire::Wire(QObject *parent, Port *ip, Port *op) : QObject(parent) {
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

Port *Wire::GetInput() {
    return inputPort;
}
Port *Wire::GetOutput() {
    return outputPort;
}

} // namespace kernel
