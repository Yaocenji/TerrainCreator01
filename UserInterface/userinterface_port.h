#ifndef USERINTERFACE_PORT_H
#define USERINTERFACE_PORT_H

#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QString>

#include "../Kernel/kernel_port.h"

namespace UserInterface {

/// 前置声明
class Node;

/// 节点的种类
enum class PortType { Input, Param, Output };

enum class PortChooseState {
    // 空
    NoneChosen,
    // 悬浮
    Suspension,
    // 选中
    Chosen,
    // 连接
    Linked
};

class Port : public QObject {
    Q_OBJECT

protected:
    /// 目标核接口
    Kernel::Port *targetPort;
    /// 父节点
    Node *parentNode;

protected:
    /// 该接口的类型
    PortType type;
    /// 该接口的状态
    PortChooseState state;
    /// 该接口的名字
    QString name;
    /// 接口相对父节点位置（父节点以左上角为中心，接口以圆心为中心）
    QPointF localPos;

public:
    /// 设置名称
    void SetName(QString name);
    /// 设置相对位置
    void SetLocalPos(QPointF p);
    /// 计算全局坐标
    QPointF GetWorldPos();
    /// 计算并设置相对位置，传入参数：接口的序号
    void CalSetNodeLocalPosition(int ordinal);

public:
    /// 绘制
    void Draw(QPainter &p);
    /// 点击检测
    bool ClickDetect(QPointF &pos, Port *&clickedPort);

public:
    explicit Port(QObject *parent = nullptr, Kernel::Port *tar = nullptr,
                  Node *parentN = nullptr, PortType ty = PortType::Input);
    explicit Port(QObject *parent = nullptr, Kernel::Port *tar = nullptr,
                  Node *parentN = nullptr, PortType ty = PortType::Input,
                  QString nam = "");
    explicit Port(QObject *parent = nullptr, Kernel::Port *tar = nullptr,
                  Node *parentN = nullptr, PortType ty = PortType::Input,
                  QString nam = "", QPointF locPos = QPointF(0.0, 0.0));
    explicit Port(QObject *parent = nullptr, Kernel::Port *tar = nullptr,
                  Node *parentN = nullptr, PortType ty = PortType::Input,
                  QString nam = "", int ordinal = 0);

signals:
};

} // namespace UserInterface
#endif // USERINTERFACE_PORT_H
