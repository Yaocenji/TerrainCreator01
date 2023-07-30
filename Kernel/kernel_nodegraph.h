#ifndef KERNEL_NODEGRAPH_H
#define KERNEL_NODEGRAPH_H

#include <QObject>
#include <QVector>

namespace kernel {

class Node;
class PerlinNoise_Node;
class Wire;

/// 节点图的计算抽象层，是nodelayout的后端，和宏节点能交叉递归
class NodeGraph : public QObject {
    Q_OBJECT
protected:
    /// 是否是子图
    bool isSubgraph;
    /// 如果是子图，那么所属的节点
    Node *ownerNode;

protected:
    /// 节点表
    QVector<Node *> nodes;
    /// 连线表
    QVector<Wire *> wires;

public:
    /**
     * @brief 构造函数
     * @param 父（默认：空）
     * @param 是否是子图（默认：否）
     * @param 如果是子图，那么所属节点（默认：空）
     */
    explicit NodeGraph(QObject *parent = nullptr, bool iSg = false,
                       Node *oN = nullptr);

signals:
};
} // namespace kernel
#endif // KERNEL_NODEGRAPH_H
