#include "render_terrainmesh.h"

namespace Render {

TerrainMesh::TerrainMesh(QObject *parent, int tilesize)
    : QObject{parent}, tileSize(tilesize) {
    vbos.resize(0);
    vaos.resize(0);
    ebos.resize(0);
}

void TerrainMesh::deleteMesh(QOpenGLFunctions_4_5_Core &f) {
    for (auto &i : vbos) {
        for (auto &j : i)
            if (j != nullptr) {
                delete j;
                j = nullptr;
            }
        i.clear();
    }
    vbos.clear();
    for (auto &i : ebos) {
        for (auto &j : i)
            if (j != nullptr) {
                delete j;
                j = nullptr;
            }
        i.clear();
    }
    ebos.clear();
    for (auto &i : vaos) {
        for (auto &j : i)
            if (j != nullptr) {
                delete j;
                j = nullptr;
            }
        i.clear();
    }
    vaos.clear();
}

void TerrainMesh::recreateMesh(QOpenGLFunctions_4_5_Core &f) {
    deleteMesh(f);

    int tileNum = globalinfo::TerrainGrid / tileSize;

    /// 数据
    vbos.resize(tileNum + 1);
    vaos.resize(tileNum + 1);
    ebos.resize(tileNum + 1);

    /// 一个tile内的顶点数
    int vertNum = tileSize + 1;
    /// 一个tile内的矩形数
    int rectNum = tileSize;

    /// 一格grid的大小
    float unit = globalinfo::TerrainSize / globalinfo::TerrainGrid;

    /// 顶点原始数据
    float *vertices = new float[vertNum * vertNum * 2];
    /// 三角形索引原始数据
    unsigned int *indices = new unsigned int[rectNum * rectNum * 6];

    /// 三角形索引原始数据生成，生成完毕后不用每次生成tile时改变
    for (int z = 0; z < rectNum; z++) {
        for (int x = 0; x < rectNum; x++) {
            int iindex = (z * rectNum + x) * 6;
            int vindex = z * vertNum + x;
            indices[iindex + 0] = vindex + 0;
            indices[iindex + 1] = vindex + vertNum + 1;
            indices[iindex + 2] = vindex + 1;
            indices[iindex + 3] = vindex + 0;
            indices[iindex + 4] = vindex + vertNum;
            indices[iindex + 5] = vindex + vertNum + 1;
        }
    }

    /// 循环遍历tile行列，生成tile
    for (int line = 0; line < tileNum; ++line) {
        for (int column = 0; column < tileNum; ++column) {
            /// 计算该tile的偏移
            float offset_x =
                (float(line) / float(tileNum) - 0.5f) * globalinfo::TerrainSize;
            float offset_z = (float(column) / float(tileNum) - 0.5f) *
                             globalinfo::TerrainSize;

            /// 更新vert数组
            for (int z = 0; z < vertNum; z++) {
                for (int x = 0; x < vertNum; x++) {
                    int index = (z * vertNum + x) * 2;
                    vertices[index + 0] = x * unit + offset_x;
                    vertices[index + 1] = z * unit + offset_z;
                }
            }

            QOpenGLBuffer *vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
            QOpenGLBuffer *ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
            QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();

            vbo->create();
            ebo->create();
            vao->create();

            vao->bind();
            vbo->bind();
            vbo->allocate(vertices, sizeof(float) * (vertNum * vertNum * 2));
            // 绑定EBO，分配索引
            ebo->bind();
            ebo->allocate(indices,
                          sizeof(unsigned int) * (rectNum * rectNum * 6));

            f.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                                    (void *)0);
            f.glEnableVertexAttribArray(0);

            vao->release();
            vbo->release();
            ebo->release();

            vbos[line].push_back(vbo);
            ebos[line].push_back(ebo);
            vaos[line].push_back(vao);
        }
    }

    delete[] indices;
    delete[] vertices;

    // TODO
    // X和Z上界的“壁”缺少，补充生成两个面
    float *addVert = new float[(globalinfo::TerrainGrid + 1) * 2 * 2];
    float *addIndex = new float[globalinfo::TerrainGrid * 6];

    /// 三角形索引原始数据生成，生成完毕后不用每次生成tile时改变
    for (int i = 0; i < globalinfo::TerrainGrid; i++) {
        int iindex = i * 6;
        int vindex = i;
        addIndex[iindex + 0] = vindex + 0;
        addIndex[iindex + 1] = vindex + globalinfo::TerrainGrid + 1 + 1;
        addIndex[iindex + 2] = vindex + 1;
        addIndex[iindex + 3] = vindex + 0;
        addIndex[iindex + 4] = vindex + globalinfo::TerrainGrid + 1;
        addIndex[iindex + 5] = vindex + globalinfo::TerrainGrid + 1 + 1;
    }

    for (int i = 0; i < globalinfo::TerrainGrid + 1; ++i) {
        addVert[2 * i] = (i / float(globalinfo::TerrainGrid) - 0.5f) *
                         globalinfo::TerrainSize;
        addVert[2 * i + 1] = -0.5 * globalinfo::TerrainSize;

        addVert[2 * i + globalinfo::TerrainGrid + 1] =
            (i / float(globalinfo::TerrainGrid) - 0.5f) *
            globalinfo::TerrainSize;
        addVert[2 * i + 1 + globalinfo::TerrainGrid + 1] =
            -0.5 * globalinfo::TerrainSize - unit;
    }

    QOpenGLBuffer *vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer *ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vbo->create();
    ebo->create();
    vao->create();
    vao->bind();
    vbo->bind();
    vbo->allocate(addVert,
                  sizeof(float) * ((globalinfo::TerrainGrid + 1) * 2 * 2));
    ebo->bind();
    ebo->allocate(addIndex,
                  sizeof(unsigned int) * (globalinfo::TerrainGrid * 6));
    f.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                            (void *)0);
    f.glEnableVertexAttribArray(0);
    vao->release();
    vbo->release();
    ebo->release();
    vbos[vbos.size() - 1].push_back(vbo);
    ebos[ebos.size() - 1].push_back(ebo);
    vaos[vaos.size() - 1].push_back(vao);

    for (int i = 0; i < globalinfo::TerrainGrid + 1; ++i) {
        addVert[2 * i] = -0.5 * globalinfo::TerrainSize;
        addVert[2 * i + 1] = (i / float(globalinfo::TerrainGrid) - 0.5f) *
                             globalinfo::TerrainSize;

        addVert[2 * i + globalinfo::TerrainGrid + 1] =
            -0.5 * globalinfo::TerrainSize - unit;
        addVert[2 * i + 1 + globalinfo::TerrainGrid + 1] =
            (i / float(globalinfo::TerrainGrid) - 0.5f) *
            globalinfo::TerrainSize;
    }

    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vao = new QOpenGLVertexArrayObject();
    vbo->create();
    ebo->create();
    vao->create();
    vao->bind();
    vbo->bind();
    vbo->allocate(addVert,
                  sizeof(float) * ((globalinfo::TerrainGrid + 1) * 2 * 2));
    ebo->bind();
    ebo->allocate(addIndex,
                  sizeof(unsigned int) * (globalinfo::TerrainGrid * 6));
    f.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                            (void *)0);
    f.glEnableVertexAttribArray(0);
    vao->release();
    vbo->release();
    ebo->release();
    vbos[vbos.size() - 1].push_back(vbo);
    ebos[ebos.size() - 1].push_back(ebo);
    vaos[vaos.size() - 1].push_back(vao);

    delete[] addVert;
    delete[] addIndex;
}

void TerrainMesh::drawMesh(QOpenGLFunctions_4_5_Core &f) {
    /// 一个tile内的矩形数
    int rectNum = tileSize;

    for (auto &i : vaos) {
        for (auto &j : i) {
            j->bind();
            f.glDrawElements(GL_TRIANGLES, rectNum * rectNum * 6,
                             GL_UNSIGNED_INT, 0);
        }
    }
}

} // namespace Render
