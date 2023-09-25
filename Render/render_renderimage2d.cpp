#include "render_renderimage2d.h".h "

namespace Render {

RenderImage2D::RenderImage2D(QObject *parent) : QObject{parent} {
    image = 0;
}

RenderImage2D::~RenderImage2D() {
    deleteRenderImage2D();
}

void RenderImage2D::deleteRenderImage2D(QOpenGLFunctions_4_5_Core &f) {
    if (image != 0) {
        f.glDeleteTextures(1, &image);
        image = 0;
    }
}

void RenderImage2D::recreateRenderImage2D(int width, int height,
                                          QOpenGLFunctions_4_5_Core &f) {
    // 先删除已有的buffer
    deleteRenderImage2D(f);

    // 生成image2D
    f.glGenTextures(1, &image);
    f.glBindTexture(GL_TEXTURE_2D, image);
    f.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, width, height);
    f.glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderImage2D::bind(unsigned int location, QOpenGLFunctions_4_5_Core &f) {
    f.glBindImageTexture(location, image, 0, GL_FALSE, 0, GL_READ_WRITE,
                         GL_RGBA32F);
}

void RenderImage2D::clear(QOpenGLFunctions_4_5_Core &f) {
    // TODO
}

GLuint RenderImage2D::getImage() {
    return image;
}

} // namespace Render
