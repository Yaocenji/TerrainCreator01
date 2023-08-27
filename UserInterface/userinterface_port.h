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
class Wire;

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
    /// 目标kernel接口
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
    /// 与接口相连的连线数量
    unsigned int linkedNumber;

public:
    /// 设置名称
    void SetName(QString name);
    /// 获取名字
    QString &GetName();
    /// 设置相对位置
    void SetLocalPos(QPointF p);
    /// 计算全局坐标
    QPointF GetWorldPos();
    /// 计算并设置相对位置，传入参数：接口的序号
    void CalSetNodeLocalPosition(int ordinal);
    /// 获取目标抽象节点
    Kernel::Port *GetTargetKernelPort();
    /// 设置接口状态
    void SetPortChooseState(PortChooseState state);
    /// 获取接口状态
    PortChooseState GetPortChooseState();
    /// 连接
    void Link();
    /// 取消链接
    void Unlink();
    /// 更新连接状态
    void PortLinkUpdate();
    /// 根据列表更新连接状态
    void PortLinkUpdateByWiresList(QVector<Wire *> &Wires);

public:
    /// 绘制
    void Draw(QPainter &p);
    /// 点击检测
    bool ClickDetect(QPointF &pos, Port *&clickedPort);

    /// 节点状态更新1：鼠标悬浮
    void PortSuspensionUpdate(Port *tar);

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
