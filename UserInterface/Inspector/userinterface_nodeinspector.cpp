#include "userinterface_nodeinspector.h"

#include "../userinterface_node.h"
#include "../userinterface_nodeeditorterminal.h"
#include "../userinterface_nodegraph.h"

namespace UserInterface {

NodeInspector::NodeInspector(NodeEditorTerminal *tar, QWidget *parent)
    : QWidget(parent), targetTerminal(tar) {
    //    table = new QTableWidget(this);
    //    table->resize(rect().size());
    items.resize(0);
    // 启动计时器
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(33);
    timer->start();
    // 根据计时器信号更新画面
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));

    //    QSlider *slider = new QSlider(this);

    //    NodeInspectorItem *item = new NodeInspectorItem(this);
}

void NodeInspector::TimerUpdate() {
    // 修正大小
    //    if (table->size() != rect().size()) {
    //        table->resize(rect().size());
    //    }
    // 修正内容
    static Node *oldtargetNode = targetNode;
    targetNode = targetTerminal->targetNodeGraph->ChosenNode().size() > 0
                     ? targetTerminal->targetNodeGraph->ChosenNode()[0]
                     : nullptr;
    if (oldtargetNode != targetNode) {
        initTable();
        oldtargetNode = targetNode;
    }
}

bool NodeInspector::initTable() {
    if (targetNode == nullptr) {
        qDebug() << "错误：targetNode为空指针";
        return false;
    }

    clearTable();

    int row_num = targetNode->targetNode->ParamPorts.size() +
                  targetNode->targetNode->nonPortParams.size();
    // 设置内容
    row_num = 0;
    for (auto i = 0; i < targetNode->targetNode->ParamPorts.size(); i++) {
        NodeInspectorItem *newItem = new NodeInspectorItem(this);
        newItem->DeleteAllController();
        newItem->SetTargetParamType(targetNode->targetNode,
                                    ItemTargetParamType::PortParam, i);
        //        table->setCellWidget(row_num, 0, newItem);
        //        table->resizeRowToContents(i);
        items.push_back(newItem);
        newItem->ResetControllerGeometry();
        row_num++;
    }
    for (auto i = 0; i < targetNode->targetNode->nonPortParams.size(); i++) {
        NodeInspectorItem *newItem = new NodeInspectorItem(this);
        newItem->DeleteAllController();
        newItem->SetTargetParamType(targetNode->targetNode,
                                    ItemTargetParamType::NonPortParam, i);
        //        table->setCellWidget(row_num, 0, newItem);
        //        table->resizeRowToContents(i);
        items.push_back(newItem);
        newItem->ResetControllerGeometry();
        row_num++;
    }
    resetTablePos();

    //    table->clear();
    // 1列n行

    //    table->setColumnCount(1);
    //    table->setRowCount(row_num);
    //    // 设置行列宽度
    //    table->setColumnWidth(0, 600);
    //    for (int i = 0; i < row_num; i++) {
    //        table->setRowHeight(i, 50);
    //    }
    //    // 设置表头不可见
    //    table->verticalHeader()->setVisible(false);
    //    table->horizontalHeader()->setVisible(false);

    //    // 表格不可被选中
    //    table->setSelectionMode(QAbstractItemView::NoSelection);

    return true;
}

void NodeInspector::clearTable() {
    for (auto &i : items) {
        delete i;
    }
    items.clear();
}

void NodeInspector::resetTablePos() {
    for (int i = 0; i < items.size(); i++) {
        items[i]->setGeometry(
            rect().center().x() - globalui::node_inspector_item_size.x() / 2.0,
            globalui::node_inspector_item_topmargin +
                i * globalui::node_inspector_item_size.y(),
            globalui::node_inspector_item_size.x(),
            globalui::node_inspector_item_size.y());

        items[i]->ResetControllerGeometry();
        items[i]->show();
    }
}

} // namespace UserInterface
