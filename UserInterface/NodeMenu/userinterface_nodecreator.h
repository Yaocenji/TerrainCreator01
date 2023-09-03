#ifndef USERINTERFACE_NODECREATOR_H
#define USERINTERFACE_NODECREATOR_H

#include <QBrush>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QVector>

#include "Kernel/kernel_element_inc.h"

namespace UserInterface {

/// 前置声明
class NodeSubMenu;

enum class NodeCreatorState { None, Hovered };

class NodeCreator : public QObject {
    Q_OBJECT

public:
    static QVector<KernelNodeConstructorPtr> nodeConstructors;

public:
    /// 所属的nodeSubMenu
    NodeSubMenu *targetNodeSubMenu;

public:
    /// 一个节点的构造函数指针的索引，创建节点时读取并调用
    int nodeConstructorEle;

public:
    /// 状态
    NodeCreatorState state;
    /// 名称
    QString title;
    /// 描述
    QString description;
    /// 背景图
    QImage image;
    /// 序号
    unsigned int number;
    /// 位置
    QRect rect();

public:
    /// 绘制
    void Draw(QPainter &p);
    /// 范围检测
    bool ClickDetect(QPointF pos);

public:
    explicit NodeCreator(QObject *parent = nullptr, NodeSubMenu *tar = nullptr,
                         KernelNodeConstructorPtr cnstrctr = nullptr,
                         unsigned int num = 0, QString nam = "",
                         QString desc = "", QString imagefilePath = "");

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODECREATOR_H
