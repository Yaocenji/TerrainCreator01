#ifndef USERINTERFACE_NODEMENU_H
#define USERINTERFACE_NODEMENU_H

#include <QApplication>
#include <QBrush>
#include <QByteArray>
#include <QDrag>
#include <QEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QWidget>

namespace UserInterface {

/// 前置声明
class NodeSubMenu;
class NodeCreator;
class NodeMenuButton;

/// 创建节点的菜单
class NodeMenu : public QWidget {
    Q_OBJECT

public:
    /// submenu所占的矩形
    QRect subMenuRect();

public:
    /// 当前选中的submenu
    NodeSubMenu *chosenSubMenu;
    /// 当前选中的menubutton
    NodeMenuButton *chosenMenuButton;
    /// 所有的submenu
    QVector<NodeSubMenu *> subMenus;
    /// 所有的menubutton
    QVector<NodeMenuButton *> menuButtons;

public:
    bool isPressing;
    /// 鼠标按下位置
    QPointF mousePressPos;
    /// 鼠标抬起位置
    QPointF mouseReleasePos;
    /// 鼠标上一帧的位置
    QPointF mouseOldPos;
    /// 点击了菜单栏左边或右边
    bool pressModel;

public:
    /// 摄像机信息 左
    int buttonBarCamPos;
    /// 摄像机信息 右
    int subMenuCamPos;
    /// 摄像机修正
    QPointF camOffset(QPointF p);
    /// 摄像机修正
    QPoint camOffset(QPoint p);

public:
    /// 初始化menu
    void InitMenu();
    /// 对menu的操作函数
    /// 添加一个subMenu
    void addSubMenu();
    /// 点击选择某个menuButton 1
    bool chooseMenuButton(int ch);
    /// 点击选择某个menuButton 2
    bool chooseMenuButton(NodeMenuButton *ch);
    /// 选择某个submenu 1
    bool chooseSubMenu(int ch);
    /// 选择某个submenu 2
    bool chooseSubMenu(NodeSubMenu *ch);

public:
    void paintEvent(QPaintEvent *event) override;

public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    explicit NodeMenu(QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_NODEMENU_H
