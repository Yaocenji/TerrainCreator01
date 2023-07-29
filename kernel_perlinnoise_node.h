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
    /// 初始化GL，分配显存
    void InitGL(QOpenGLFunctions_4_5_Core &f) override;
    /// 分配显存
    void Allocate(QOpenGLFunctions_4_5_Core &f) override;
    /// 运行节点
    void RunNode(QOpenGLFunctions_4_5_Core &f) override;
    /// 选择并显示节点
    void Choose(QOpenGLFunctions_4_5_Core &f) override;
    /// 释放
    void Release(QOpenGLFunctions_4_5_Core &f) override;
    unsigned int static test(QOpenGLFunctions_4_5_Core &f);

public:
    Kernel_PerlinNoise_Node(QObject *parent = nullptr);
};
} // namespace kernel
#endif // KERNEL_PERLINNOISE_NODE_H
