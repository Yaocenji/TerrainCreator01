#ifndef USERINTERFACE_NODEINSPECTOR_H
#define USERINTERFACE_NODEINSPECTOR_H

#include <QObject>
#include <QWidget>

namespace Kernel {
// 前置声明
class Node;
} // namespace Kernel

namespace UserInterface {

// 前置声明
class Node;

/// 节点选项控制面板的基类
class NodeInspector : public QWidget {
    Q_OBJECT
public:
    explicit NodeInspector(QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEINSPECTOR_H
