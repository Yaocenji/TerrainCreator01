#ifndef RENDER_PARALLELLIGHT_H
#define RENDER_PARALLELLIGHT_H

#include <QObject>
#include <QVector3D>

#include "render_light.h"

namespace Render {

/// 平行光
class ParallelLight : public Light {
    Q_OBJECT
protected:
    /// 光线方向，该值和实际意义上的“光线方向”是相反向量
    QVector3D direction;

public:
    /// 设置光线方向
    void setDirection(QVector3D dir);

    /// 获取光线方向
    QVector3D getDirection();

public:
    explicit ParallelLight(QObject *parent = nullptr,
                           QVector3D dir = QVector3D(1, 0.5, 1).normalized(),
                           QColor col = Qt::white, float stgth = 1.0f);

signals:
};

} // namespace Render

#endif // RENDER_PARALLELLIGHT_H
