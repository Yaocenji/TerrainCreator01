#ifndef RENDER_RENDERTEXTURE_H
#define RENDER_RENDERTEXTURE_H

#include <QDebug>
#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QVector>

#include "Global/globalgl.h"
#include "Global/globalrender.h"

namespace Render {

class RenderTexture : public QObject {
    Q_OBJECT
public:
    /// 目标fbo
    GLuint frameBuffer;
    /// 目标颜色 color buffer(texture)
    GLuint colorBuffer;
    /// 目标深度 depth buffer
    GLuint depthBuffer;
    // 不支持模板
    /// 是否拥有depth纹理
    const bool hasDepthBuffer;

public:
    /// 完全删除当前buffer缓存
    void deleteRenderTexture(
        QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 创建（并覆盖）新RT
    void recreateRenderTexture(
        int width, int height,
        QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 绑定该rt为目标渲染对象
    void bind(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 渲染不透明物体前，首先清理
    void clear(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));

    /// 获得颜色纹理
    GLuint colorTexture();
    /// 获得深度纹理
    GLuint depthTexture();

public:
    explicit RenderTexture(QObject *parent = nullptr, bool hasDep = false);
    ~RenderTexture();

signals:
};

} // namespace Render
#endif // RENDER_RENDERTEXTURE_H
