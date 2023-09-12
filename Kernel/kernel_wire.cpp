#include "kernel_wire.h"

#include "kernel_port.h"

namespace Kernel {

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

// Wire::~Wire() {
//     // 全部置空即可
//     inputPort = nullptr;
//     outputPort = nullptr;
//     targetUIWire = nullptr;
// }

Port *Wire::GetInput() {
    return inputPort;
}
Port *Wire::GetOutput() {
    return outputPort;
}

} // namespace Kernel
