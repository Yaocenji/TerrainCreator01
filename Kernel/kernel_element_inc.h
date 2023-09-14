#ifndef KERNEL_NODES_INC_H
#define KERNEL_NODES_INC_H

#include <QObject>

// 用于统一include节点们
// 自定义节点
#include "Nodes/Filter/kernel_clamp_node.h"
#include "Nodes/Filter/kernel_invert_node.h"
#include "Nodes/Filter/kernel_terrace_node.h"
#include "Nodes/Generator/kernel_perlinnoise_node.h"
#include "Nodes/Math/kernel_number_node.h"
// 核心组件
#include "kernel_node.h"
#include "kernel_nodegraph.h"
#include "kernel_port.h"
#include "kernel_wire.h"

typedef Kernel::Node *(*KernelNodeConstructorPtr)(QObject *,
                                                  Kernel::NodeGraph *);

namespace kernel {

class kernel_element_inc : public QObject {
    Q_OBJECT
public:
    explicit kernel_element_inc(QObject *parent = nullptr);

signals:
};

} // namespace kernel

#endif // KERNEL_NODES_INC_H
