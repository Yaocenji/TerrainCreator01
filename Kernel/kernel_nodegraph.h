#ifndef KERNEL_NODEGRAPH_H
#define KERNEL_NODEGRAPH_H

#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QVector>

#include "kernel_element_forwarddec.cpp"

namespace Kernel {

ForwardDeclarationWithoutGraph;

/// 节点图的计算抽象层，是nodelayout的后端，和宏节点能交叉递归
class NodeGraph : public QObject {
    Q_OBJECT
protected:
    /// 是否是子图
    bool isSubgraph;
    /// 如果是子图，那么所属的节点
    Node *parentNode;
    /// 是否计算完成
    bool isFinished;

public:
    /// 子图系统：输入节点
    ///
    /// 子图系统：输出节点
    ///
    /// 节点表
    QVector<Node *> nodes;
    /// 连线表
    QVector<Wire *> wires;

public:
    /**
     * @brief 添加节点
     * @param 节点指针
     */
    void AddNode(Node *n);
    /**
     * @brief 添加连线
     * @param 接口1
     * @param 接口2
     * @return 返回值：如果连接成功，返回新创建的连线；否则返回nullptr
     */
    Wire *LinkWire(Port *a, Port *b);
    /**
     * @brief 删除连线
     * @param 目标连线
     * @return
     */
    bool DeleteWire(Wire *tar);
    /**
     * @brief 删除节点
     * @param 要删除的目标节点
     * @return
     */
    bool DeleteNode(Node *tar);

    Node *GetParentMacroNode();

public:
    // 运行节点图
    bool RunNodeGraph(QOpenGLFunctions_4_5_Core &f);

public:
    /**
     * @brief 构造函数
     * @param 父（默认：空）
     * @param 是否是子图（默认：否）
     * @param 如果是子图，那么所属节点（默认：空）
     */
    explicit NodeGraph(QObject *parent, bool iSg = false, Node *oN = nullptr);
    // 析构函数
    ~NodeGraph();

signals:
};
} // namespace Kernel
#endif // KERNEL_NODEGRAPH_H
