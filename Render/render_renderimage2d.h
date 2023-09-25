#ifndef RENDER_RENDERIMAGE2D_H
#define RENDER_RENDERIMAGE2D_H

#include <QDebug>
#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QVector>

#include "Global/globalgl.h"
#include "Global/globalrender.h"

namespace Render {

class RenderImage2D : public QObject {
    Q_OBJECT
public:
    /// 目标image
    GLuint image;

public:
    /// 完全删除当前buffer缓存
    void deleteRenderImage2D(
        QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 创建（并覆盖）新RT
    void recreateRenderImage2D(
        int width, int height,
        QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 绑定该rt为目标渲染对象
    void bind(unsigned int location,
              QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));

    /// 渲染不透明物体前，首先清理
    void clear(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));

    /// 获得颜色纹理
    GLuint getImage();

public:
    explicit RenderImage2D(QObject *parent = nullptr);
    ~RenderImage2D();

signals:
};

} // namespace Render
#endif // RENDER_RENDERIMAGE2D_H
