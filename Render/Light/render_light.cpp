#include "render_light.h"

namespace Render {

Light::Light(QObject *parent, QColor col, float stgth)
    : QObject{parent}, color(col), strength(stgth) {
}

void Light::setColor(QColor col) {
    color = col;
}

void Light::setStrength(float stren) {
    strength = stren;
}

QVector3D Light::getColor() {
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}
float Light::getStrength() {
    return strength;
}

} // namespace Render
