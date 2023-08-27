#include "userinterface_wire.h"

#include "../Global/globalui.h"
#include "userinterface_port.h"

namespace UserInterface {

Wire::Wire(QObject *parent, Kernel::Wire *tar, Port *lp1, Port *lp2)
    : QObject(parent), targetWire(tar) {
    tar->targetUIWire = this;
    linkedPort[0] = lp1;
    linkedPort[1] = lp2;
}

void Wire::Draw(QPainter &p) {
    DrawWire(p, linkedPort[0]->GetWorldPos(), linkedPort[1]->GetWorldPos());
}

void Wire::DrawWire(QPainter &p, QPointF pos1, QPointF pos2) {
    p.setPen(QPen(globalui::wire_color, 5));
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
