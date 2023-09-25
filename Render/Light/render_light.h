#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include <QColor>
#include <QObject>
#include <QVector3D>

namespace Render {

class Light : public QObject {
    Q_OBJECT
protected:
    /// 光源颜色
    QColor color;
    /// 光源强度
    float strength;

public:
    /// 设置光源颜色
    void setColor(QColor col);
    /// 设置光源强度
    void setStrength(float stren);

    /// 获取光源颜色
    QVector3D getColor();
    /// 获取光源强度
    float getStrength();

public:
    explicit Light(QObject *parent = nullptr, QColor col = Qt::white,
                   float stgth = 1.0f);

signals:
};

} // namespace Render

#endif // RENDER_LIGHT_H
