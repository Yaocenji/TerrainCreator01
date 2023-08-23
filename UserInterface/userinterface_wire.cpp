#include "userinterface_wire.h"

#include "userinterface_port.h"

namespace UserInterface {

Wire::Wire(QObject *parent, Kernel::Wire *tar, Port *lp1, Port *lp2)
    : QObject(parent), targetWire(tar) {
    linkedPort[0] = lp1;
    linkedPort[1] = lp2;
}

void Wire::Draw(QPainter &p) {
    p.setPen(QPen(QColor(210, 210, 210), 5));
    QPointF beginPos = QPointF(linkedPort[0]->GetWorldPos().x(),
                               linkedPort[0]->GetWorldPos().y());
    QPointF c1 = QPointF((linkedPort[0]->GetWorldPos().x() * 3.0f +
                          linkedPort[1]->GetWorldPos().x()) /
                             4.0f,
                         linkedPort[0]->GetWorldPos().y());
    QPointF midPos = QPointF(
        (linkedPort[0]->GetWorldPos().x() + linkedPort[1]->GetWorldPos().x()) /
            2.0f,
        (linkedPort[0]->GetWorldPos().y() + linkedPort[1]->GetWorldPos().y()) /
            2.0f);
    QPointF c2 = QPointF((linkedPort[0]->GetWorldPos().x() +
                          linkedPort[1]->GetWorldPos().x() * 3.0f) /
                             4.0f,
                         linkedPort[1]->GetWorldPos().y());
    QPointF endPos = QPointF(linkedPort[1]->GetWorldPos().x(),
                             linkedPort[1]->GetWorldPos().y());
    QPainterPath path;
    path.moveTo(beginPos);
    path.quadTo(c1, midPos);
    path.quadTo(c2, endPos);
    p.drawPath(path);
}

} // namespace UserInterface
