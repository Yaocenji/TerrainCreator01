#ifndef RENDER_TERRAINMESH_H
#define RENDER_TERRAINMESH_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector>

#include "Global/globalgl.h"
#include "Global/globalinfo.h"

namespace Render {

/// 地形网格
class TerrainMesh : public QObject {
    Q_OBJECT
public:
    int tileSize;

public:
    /// VBO数组
    QVector<QVector<QOpenGLBuffer *> > vbos;
    /// EBO数组
    QVector<QVector<QOpenGLBuffer *> > ebos;
    /// VAO数组
    QVector<QVector<QOpenGLVertexArrayObject *> > vaos;

public:
    /// 删除已有的网格信息
    void deleteMesh(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 创建新的网格信息
    void recreateMesh(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));
    /// 绘制函数
    void drawMesh(QOpenGLFunctions_4_5_Core &f = *(globalgl::thisContext));

public:
    explicit TerrainMesh(QObject *parent = nullptr, int tilesize = 1024);

signals:
};

} // namespace Render

#endif // RENDER_TERRAINMESH_H
