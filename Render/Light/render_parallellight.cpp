#include "render_parallellight.h"

namespace Render {

ParallelLight::ParallelLight(QObject *parent, QVector3D dir, QColor col,
                             float stgth)
    : Light{parent, col, stgth}, direction(dir) {
}

void ParallelLight::setDirection(QVector3D dir) {
    if (dir.length() == 0)
        return;
    else
        direction = dir.normalized();
}

QVector3D ParallelLight::getDirection() {
    return direction.normalized();
}

} // namespace Render
