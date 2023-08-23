#ifndef KERNEL_NODES_INC_H
#define KERNEL_NODES_INC_H

#include <QObject>

// 用于统一include节点们
#include "Nodes/Filter/kernel_clamp_node.h"
#include "Nodes/Generator/kernel_perlinnoise_node.h"
#include "kernel_node.h"
#include "kernel_nodegraph.h"
#include "kernel_port.h"
#include "kernel_wire.h"

namespace kernel {

class kernel_element_inc : public QObject {
    Q_OBJECT
public:
    explicit kernel_element_inc(QObject *parent = nullptr);

signals:
};

} // namespace kernel

#endif // KERNEL_NODES_INC_H
