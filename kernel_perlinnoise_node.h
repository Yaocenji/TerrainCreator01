#ifndef KERNEL_PERLINNOISE_NODE_H
#define KERNEL_PERLINNOISE_NODE_H

#include <QObject>
#include <QOpenGLShader>

#include "kernel_node.h"

namespace kernel {
/// 柏林噪声节点————抽象层
class Kernel_PerlinNoise_Node : public Node {
    Q_OBJECT
protected:
public:
    void InitGL(QOpenGLFunctions_4_5_Core &f) override;
    void RunNode(QOpenGLFunctions_4_5_Core &f) override;
    void Choose(QOpenGLFunctions_4_5_Core &f) override;
    void Release(QOpenGLFunctions_4_5_Core &f) override;
    unsigned int static test(QOpenGLFunctions_4_5_Core &f);

public:
    Kernel_PerlinNoise_Node(QObject *parent = nullptr);
};
} // namespace kernel
#endif // KERNEL_PERLINNOISE_NODE_H
