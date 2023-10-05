#ifndef RENDER_RECTMESH_H
#define RENDER_RECTMESH_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector>

#include "Global/globalgl.h"
#include "Global/globalinfo.h"

namespace Render {

/// xz平面的正方形
class RectMesh : public QObject {
    Q_OBJECT
public:
    float rectSize;

public:
    /// VBO数组
    QOpenGLBuffer *vbo;
    /// EBO数组
    QOpenGLBuffer *ebo;
    /// VAO数组
    QOpenGLVertexArrayObject *vao;

public:
    /// 删除已有的网格信息
    void deleteMesh(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 创建新的网格信息
    void recreateMesh(float rectS,
                      QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 绘制函数
    void drawMesh(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));

public:
    explicit RectMesh(QObject *parent = nullptr);

signals:
};

} // namespace Render
#endif // RENDER_RECTMESH_H
