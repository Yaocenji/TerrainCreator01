
#include "userinterface_port.h"

#include "../Global/globalui.h"
#include "userinterface_node.h"
#include "userinterface_wire.h"

namespace UserInterface {

Port::Port(QObject *parent, Kernel::Port *tar, Node *parentN, PortType ty)
    : QObject(parent),
      targetPort(tar),
      parentNode(parentN),
      type(ty),
      linkedNumber(0) {
    targetPort->targetUIPort = this;
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
      name(nam),
      linkedNumber(0) {
    targetPort->targetUIPort = this;
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
      localPos(locPos),
      linkedNumber(0) {
    targetPort->targetUIPort = this;
    state = PortChooseState::NoneChosen;
}

Port::Port(QObject *parent, Kernel::Port *tar, Node *parentN, PortType ty,
           QString nam, int ordinal)
    : QObject(parent),
      targetPort(tar),
      parentNode(parentN),
      type(ty),
      name(nam),
      linkedNumber(0) {
    targetPort->targetUIPort = this;
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
        localPos = QPointF(0, globalui::input_column_found_size[0] +
                                  globalui::port_interval_size * ordinal);
    } else if (type == PortType::Output) {
        localPos = QPointF(parentNode->GetRectInfo().size().width(),
                           globalui::output_column_found_size[0] +
                               globalui::port_interval_size * ordinal);
    } else {
        localPos = QPointF(globalui::param_line_found_size[0] +
                               globalui::port_interval_size * ordinal,
                           0);
    }
}

Kernel::Port *Port::GetTargetKernelPort() {
    return targetPort;
}

void Port::SetPortChooseState(PortChooseState state) {
    this->state = state;
}

PortChooseState Port::GetPortChooseState() {
    return state;
}

void Port::Link() {
    linkedNumber++;
    PortLinkUpdate();
}

void Port::Unlink() {
    linkedNumber--;
    PortLinkUpdate();
}

void Port::Draw(QPainter &p) {
    if (linkedNumber > 0) {
        this->state = PortChooseState::Linked;
    }
    QPointF worldPos = parentNode->GetRectInfo().topLeft() + localPos;
    QPointF topLeft =
        worldPos + QPointF(-globalui::port_radius, -globalui::port_radius);
    QBrush br = p.brush();
    if (this->state == PortChooseState::Suspension) {
        p.setBrush(QColor(parentNode->color().red() / 4.0 + 128,
                          parentNode->color().green() / 4.0 + 128,
                          parentNode->color().blue() / 4.0 + 128));
        //        this->state = PortChooseState::NoneChosen;
    } else if (this->state == PortChooseState::Chosen) {
        p.setBrush(QColor(parentNode->color().red() / 8.0 + 192,
                          parentNode->color().green() / 8.0 + 192,
                          parentNode->color().blue() / 8.0 + 192));
    } else if (this->state == PortChooseState::Linked) {
        p.setBrush(globalui::wire_color);
    }
    p.drawEllipse(topLeft.x(), topLeft.y(), globalui::port_radius * 2,
                  globalui::port_radius * 2);
    p.setBrush(br);
}

bool Port::ClickDetect(QPointF &pos, Port *&clickedPort) {
    QPointF worldPos = parentNode->GetRectInfo().topLeft() + localPos;
    if (pos.x() >= worldPos.x() - globalui::port_radius * 1.5 &&
        pos.x() <= worldPos.x() + globalui::port_radius * 1.5 &&
        pos.y() >= worldPos.y() - globalui::port_radius * 1.5 &&
        pos.y() <= worldPos.y() + globalui::port_radius * 1.5) {
        clickedPort = this;

        return true;
    } else {
        return false;
    }
}

void Port::PortLinkUpdate() {
    if (linkedNumber > 0 && state != PortChooseState::Linked)
        state = PortChooseState::Linked;
    if (linkedNumber <= 0 && state == PortChooseState::Linked)
        state = PortChooseState::NoneChosen;
}

void Port::PortLinkUpdateByWiresList(QVector<Wire *> &Wires) {
    linkedNumber = 0;
    for (auto &i : Wires) {
        if (i->linkedPort[0] == this || i->linkedPort[1] == this) {
            linkedNumber++;
        }
    }
    PortLinkUpdate();
}

void Port::PortSuspensionUpdate(Port *tar) {
    if (state == PortChooseState::Chosen || state == PortChooseState::Linked) {
        return;
    }
    if (tar == this) {
        SetPortChooseState(PortChooseState::Suspension);
    } else {
        SetPortChooseState(PortChooseState::NoneChosen);
    }
}

} // namespace UserInterface
