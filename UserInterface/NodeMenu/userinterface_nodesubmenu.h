#ifndef USERINTERFACE_NODESUBMENU_H
#define USERINTERFACE_NODESUBMENU_H

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QWidget>

#include "Kernel/kernel_element_inc.h"

namespace UserInterface {

/// 前置声明nodemenu
class NodeMenu;
/// 前置声明submenu所包含的节点图标
class NodeCreator;

/// 创建节点的菜单的下级菜单
class NodeSubMenu : public QObject {
    Q_OBJECT

public:
    /// 所属的nodeMenu
    NodeMenu *targetNodeMenu;
    /// 包含的节点创建图标列表
    QVector<NodeCreator *> nodeCreators;

public:
    /// 获得矩形
    QRect rect();
    /// 添加nodeCreator
    void addNodeCreator(KernelNodeConstructorPtr cnstr, QString tit,
                        QString desc, QString imPath);
    /// 绘制
    void Draw(QPainter &p, int verticalCamOffset);

public:
    explicit NodeSubMenu(QWidget *parent = nullptr, NodeMenu *target = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODESUBMENU_H
