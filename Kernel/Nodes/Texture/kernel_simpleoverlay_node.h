#ifndef KERNEL_SIMPLEOVERLAY_NODE_H
#define KERNEL_SIMPLEOVERLAY_NODE_H

#include <QObject>
#include <QOpenGLShader>

#include "../../kernel_node.h"

namespace Kernel {

/// 阶梯节点————抽象层
class SimpleOverlay_Node : public Node {
    Q_OBJECT
protected:
public:
    /// 初始化GL，分配显存
    void InitGL(QOpenGLFunctions_4_5_Core &f) override;
    /// 运行节点
    void CalculateNode(QOpenGLFunctions_4_5_Core &f) override;
    /// 选择并显示节点
    void Choose(QOpenGLFunctions_4_5_Core &f) override;

public:
    SimpleOverlay_Node(QObject *parent = nullptr, NodeGraph *pNG = nullptr);
};

} // namespace Kernel

#endif // KERNEL_SIMPLEOVERLAY_NODE_H
