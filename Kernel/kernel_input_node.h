#ifndef KERNEL_INPUT_NODE_H
#define KERNEL_INPUT_NODE_H

#include <QObject>

#include "kernel_node.h"

namespace kernel {

class Input_Node : public Node {
    Q_OBJECT
protected:
public:
    /// 初始化GL，分配显存
    void InitGL(QOpenGLFunctions_4_5_Core &f) override;
    /// 分配显存
    void Allocate(QOpenGLFunctions_4_5_Core &f) override;
    /// 运行节点
    void CalculateNode(QOpenGLFunctions_4_5_Core &f) override;
    /// 选择并显示节点
    void Choose(QOpenGLFunctions_4_5_Core &f) override;
    /// 释放
    void Release(QOpenGLFunctions_4_5_Core &f) override;

public:
    explicit Input_Node(QObject *parent = nullptr);

signals:
};

} // namespace kernel
#endif // KERNEL_INPUT_NODE_H
