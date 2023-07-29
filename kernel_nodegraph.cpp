#include "kernel_nodegraph.h"
namespace kernel {

NodeGraph::NodeGraph(QObject *parent, bool iSg, Node *oN)
    : QObject(parent), isSubgraph(iSg), ownerNode(oN) {
}

} // namespace kernel
