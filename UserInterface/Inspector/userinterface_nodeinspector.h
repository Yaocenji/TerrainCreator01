#ifndef USERINTERFACE_NODEINSPECTOR_H
#define USERINTERFACE_NODEINSPECTOR_H

#include <QDebug>
#include <QHeaderView>
#include <QObject>
#include <QSlider>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QWidget>

#include "userinterface_nodeinspector_item.h"

namespace Kernel {
// 前置声明
class Node;
} // namespace Kernel

namespace UserInterface {

// 前置声明
class Node;
class NodeEditorTerminal;

/// 节点选项控制面板的基类
class NodeInspector : public QWidget {
    Q_OBJECT

protected:
    QTimer *timer;
public slots:
    void TimerUpdate();

public:
    /// 当前激活的节点编辑器终端
    UserInterface::NodeEditorTerminal *targetTerminal;
    /// 要调整的目标节点
    UserInterface::Node *targetNode;

public:
    /// （弃用）参数调整表格
    QTableWidget *table;
    /// （重新）初始化表格
    bool initTable();
    /// 重置表格
    void clearTable();
    /// 表格内组件重新排列位置
    void resetTablePos();
    /// 参数条目数组
    QVector<NodeInspectorItem *> items;

signals:
    /// 数值发生改变
    void OccurValueChange();

public:
    /// 抛出：数值发生改变
    void GiveSig_OccurValueChange();

public:
    explicit NodeInspector(NodeEditorTerminal *tar, QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEINSPECTOR_H
