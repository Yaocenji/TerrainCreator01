#include "render_rectmesh.h"

namespace Render {

RectMesh::RectMesh(QObject *parent) : QObject{parent} {
    vao = nullptr;
    vbo = nullptr;
    ebo = nullptr;
}

void RectMesh::deleteMesh(QOpenGLFunctions_4_5_Core &f) {
    if (vbo != nullptr) {
        delete vbo;
        vbo = nullptr;
    }
    if (vao != nullptr) {
        delete vao;
        vao = nullptr;
    }
    if (ebo != nullptr) {
        delete ebo;
        ebo = nullptr;
    }
}

void RectMesh::recreateMesh(float rectS, QOpenGLFunctions_4_5_Core &f) {
    deleteMesh(f);

    this->rectSize = rectS;

    float *vertices = new float[8];
    unsigned int *indices = new unsigned int[6];

    vertices[0] = -rectS * 0.5;
    vertices[1] = -rectS * 0.5;

    vertices[2] = +rectS * 0.5;
    vertices[3] = -rectS * 0.5;

    vertices[4] = +rectS * 0.5;
    vertices[5] = +rectS * 0.5;

    vertices[6] = -rectS * 0.5;
    vertices[7] = +rectS * 0.5;

    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;
    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 2;

    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vao = new QOpenGLVertexArrayObject();
    vbo->create();
    ebo->create();
    vao->create();
    vao->bind();
    vbo->bind();
    vbo->allocate(vertices, sizeof(float) * 8);
    ebo->bind();
    ebo->allocate(indices, sizeof(unsigned int) * 6);
    f.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                            (void *)0);
    f.glEnableVertexAttribArray(0);
    vao->release();
    vbo->release();
    ebo->release();

    delete[] vertices;
    delete[] indices;
}

void RectMesh::drawMesh(QOpenGLFunctions_4_5_Core &f) {
    vao->bind();
    f.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace Render
