#include "userinterface_wire.h"

#include "../Global/userinterface_option.h"
#include "userinterface_port.h"

namespace UserInterface {

Wire::Wire(QObject *parent, Kernel::Wire *tar, Port *lp1, Port *lp2)
    : QObject(parent), targetWire(tar) {
    linkedPort[0] = lp1;
    linkedPort[1] = lp2;
}

void Wire::Draw(QPainter &p) {
    //    p.setPen(QPen(Option::wire_color, 5));
    //    QPointF beginPos = QPointF(linkedPort[0]->GetWorldPos().x(),
    //                               linkedPort[0]->GetWorldPos().y());
    //    QPointF c1 = QPointF((linkedPort[0]->GetWorldPos().x() * 3.0f +
    //                          linkedPort[1]->GetWorldPos().x()) /
    //                             4.0f,
    //                         linkedPort[0]->GetWorldPos().y());
    //    QPointF midPos = QPointF(
    //        (linkedPort[0]->GetWorldPos().x() +
    //        linkedPort[1]->GetWorldPos().x()) /
    //            2.0f,
    //        (linkedPort[0]->GetWorldPos().y() +
    //        linkedPort[1]->GetWorldPos().y()) /
    //            2.0f);
    //    QPointF c2 = QPointF((linkedPort[0]->GetWorldPos().x() +
    //                          linkedPort[1]->GetWorldPos().x() * 3.0f) /
    //                             4.0f,
    //                         linkedPort[1]->GetWorldPos().y());
    //    QPointF endPos = QPointF(linkedPort[1]->GetWorldPos().x(),
    //                             linkedPort[1]->GetWorldPos().y());
    //    QPainterPath path;
    //    path.moveTo(beginPos);
    //    path.quadTo(c1, midPos);
    //    path.quadTo(c2, endPos);
    //    p.drawPath(path);
    DrawWire(p, linkedPort[0]->GetWorldPos(), linkedPort[1]->GetWorldPos());
}

void Wire::DrawWire(QPainter &p, QPointF pos1, QPointF pos2) {
    p.setPen(QPen(Option::wire_color, 5));
    QPointF beginPos = pos1;
    QPointF c1 = QPointF((pos1.x() * 3.0f + pos2.x()) / 4.0f, pos1.y());
    QPointF midPos =
        QPointF((pos1.x() + pos2.x()) / 2.0f, (pos1.y() + pos2.y()) / 2.0f);
    QPointF c2 = QPointF((pos1.x() + pos2.x() * 3.0f) / 4.0f, pos2.y());
    QPointF endPos = pos2;
    QPainterPath path;
    path.moveTo(beginPos);
    path.quadTo(c1, midPos);
    path.quadTo(c2, endPos);
    p.drawPath(path);
}

} // namespace UserInterface
