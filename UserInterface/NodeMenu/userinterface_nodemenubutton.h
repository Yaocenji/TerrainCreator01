#ifndef USERINTERFACE_NODEMENUBUTTON_H
#define USERINTERFACE_NODEMENUBUTTON_H

#include <QBrush>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QVector>

namespace UserInterface {

/// 前置声明
class NodeMenu;
class NodeSubMenu;

enum class NodeMenuButtonState { None, Hovered, Pressed };

class NodeMenuButton : public QObject {
    Q_OBJECT
public:
    /// 所属的nodemenu
    NodeMenu *targetNodeMenu;
    /// 是否被选中
    NodeMenuButtonState state;

public:
    /// 标题名字
    QString title;
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
    explicit NodeMenuButton(QObject *parent = nullptr, NodeMenu *tar = nullptr,
                            unsigned int num = 0, QString tit = "",
                            QString imagefilePath = "");

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEMENUBUTTON_H
