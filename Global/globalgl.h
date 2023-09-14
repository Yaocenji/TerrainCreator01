#ifndef GLOBALGL_H
#define GLOBALGL_H

#include <QOpenGLFunctions_4_5_Core>

#define DEBUG_GL \
    if (globalgl::isDebuggingGL) qDebug()

class globalgl {
public:
    /// 当前的OpenGL上下文
    static QOpenGLFunctions_4_5_Core *thisContext;

    /// 是否执行OpenGL debug模式
    static bool isDebuggingGL;

public:
    globalgl();
};

#endif // GLOBALGL_H
