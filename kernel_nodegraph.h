#ifndef KERNEL_NODEGRAPH_H
#define KERNEL_NODEGRAPH_H

#include <QObject>

#include "kernel_node.h"
#include "kernel_perlinnoise_node.h"
namespace kernel {

/// 节点图的计算抽象层，是nodelayout的后端，和宏节点能交叉递归
class NodeGraph : public QObject {
    Q_OBJECT
public:
    explicit NodeGraph(QObject *parent = nullptr);

signals:
};
} // namespace kernel
#endif // KERNEL_NODEGRAPH_H
