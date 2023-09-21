#ifndef KERNEL_BASICEROSION01_NODE_H
#define KERNEL_BASICEROSION01_NODE_H

#include <QDateTime>
#include <QObject>
#include <QOpenGLShader>
#include <QVector2D>

#include "../../kernel_node.h"

namespace Kernel {

/// 基础侵蚀
class BasicErosion01_Node : public Node {
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
    BasicErosion01_Node(QObject *parent = nullptr, NodeGraph *pNG = nullptr);
};

} // namespace Kernel

#endif // KERNEL_BASICEROSION01_NODE_H
