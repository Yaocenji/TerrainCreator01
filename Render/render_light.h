#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include <QColor>
#include <QObject>

namespace Render {

class Light : public QObject {
    Q_OBJECT
public:
    /// 光源颜色
    QColor color;
    /// 光源强度
    float strength;

public:
    explicit Light(QObject *parent = nullptr);

signals:
};

} // namespace Render

#endif // RENDER_LIGHT_H
