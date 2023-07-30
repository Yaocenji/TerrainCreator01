#include "kernel_nodegraph.h"

#include "kernel_node.h"
#include "kernel_perlinnoise_node.h"

namespace kernel {

NodeGraph::NodeGraph(QObject *parent, bool iSg, Node *oN)
    : QObject(parent), isSubgraph(iSg), ownerNode(oN) {
}

} // namespace kernel
