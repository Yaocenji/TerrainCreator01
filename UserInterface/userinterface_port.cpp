
#include "userinterface_port.h"

#include "../Global/userinterface_option.h"
#include "userinterface_node.h"

namespace UserInterface {

Port::Port(QObject *parent, Kernel::Port *tar, Node *parentN, PortType ty)
    : QObject(parent), targetPort(tar), parentNode(parentN), type(ty) {
    state = PortChooseState::NoneChosen;
    localPos = QPointF(0, 0);
    name = targetPort->name;
}

Port::Port(QObject *parent, Kernel::Port *tar, Node *parentN, PortType ty,
           QString nam)
    : QObject(parent),
      targetPort(tar),
      parentNode(parentN),
      type(ty),
      name(nam) {
    state = PortChooseState::NoneChosen;
    localPos = QPointF(0, 0);
}

Port::Port(QObject *parent, Kernel::Port *tar, Node *parentN, PortType ty,
           QString nam, QPointF locPos)
    : QObject(parent),
      targetPort(tar),
      parentNode(parentN),
      type(ty),
      name(nam),
      localPos(locPos) {
    state = PortChooseState::NoneChosen;
}

Port::Port(QObject *parent, Kernel::Port *tar, Node *parentN, PortType ty,
           QString nam, int ordinal)
    : QObject(parent),
      targetPort(tar),
      parentNode(parentN),
      type(ty),
      name(nam) {
    state = PortChooseState::NoneChosen;
    CalSetNodeLocalPosition(ordinal);
}

QPointF Port::GetWorldPos() {
    return parentNode->GetRectInfo().topLeft() + localPos;
}

void Port::SetLocalPos(QPointF p) {
    localPos = p;
}

void Port::SetName(QString name) {
    this->name = name;
}

QString &Port::GetName() {
    return name;
}

void Port::CalSetNodeLocalPosition(int ordinal) {
    if (type == PortType::Input) {
        localPos = QPointF(0, Option::input_column_found_size[0] +
                                  Option::port_interval_size * ordinal);
    } else if (type == PortType::Output) {
        localPos = QPointF(parentNode->GetRectInfo().size().width(),
                           Option::output_column_found_size[0] +
                               Option::port_interval_size * ordinal);
    } else {
        localPos = QPointF(Option::param_line_found_size[0] +
                               Option::port_interval_size * ordinal,
                           0);
    }
}

Kernel::Port *Port::GetTargetKernelPort() {
    return targetPort;
}

void Port::Draw(QPainter &p) {
    QPointF worldPos = parentNode->GetRectInfo().topLeft() + localPos;
    QPointF topLeft =
        worldPos + QPointF(-Option::port_radius, -Option::port_radius);
    p.drawEllipse(topLeft.x(), topLeft.y(), Option::port_radius * 2,
                  Option::port_radius * 2);
}

bool Port::ClickDetect(QPointF &pos, Port *&clickedPort) {
    QPointF worldPos = parentNode->GetRectInfo().topLeft() + localPos;
    if (pos.x() >= worldPos.x() - Option::port_radius &&
        pos.x() <= worldPos.x() + Option::port_radius &&
        pos.y() >= worldPos.y() - Option::port_radius &&
        pos.y() <= worldPos.y() + Option::port_radius) {
        clickedPort = this;
        return true;
    } else {
        return false;
    }
}

} // namespace UserInterface
