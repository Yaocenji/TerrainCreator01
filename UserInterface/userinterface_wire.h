#ifndef USERINTERFACE_WIRE_H
#define USERINTERFACE_WIRE_H

#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QString>

#include "../Kernel/kernel_wire.h"

namespace UserInterface {

/// 线条的选中状态
enum class WireChosenState { None, Hovered, Chosen };

/// 前置声明
class Port;

class Wire : public QObject {
    Q_OBJECT
public:
    /// 目标kernel连线
    Kernel::Wire *targetWire;
    /// 相连的接口
    Port *linkedPort[2];
    /// 该节点的状态
    WireChosenState state;

public:
    /// 用于在节点图编辑终端画板的frameBuffer中执行点击检测的随机颜色索引
    int checkColorIndex;
    /// 存储所有wire的该颜色的数组
    static QVector<QColor> checkColors;
    /// 创建一个与数组内容不相重复的color
    bool createCheckColor();
    /// 删除该wire的checkcolor
    bool deleteCheckColor();

public:
    /// 绘制
    void Draw(QPainter &p);
    /// 绘制用于交互的frameBuffer
    void DrawFrameBuffer(QPainter &p);
    /// 点击检测函数
    bool ClickDetect(QPointF pos, Wire *&clickedWire, QPixmap &fb);
    /// 提供一个绘制类wire曲线的接口供外部对象调用
    static void DrawWire(QPainter &p, QPointF pos1, QPointF pos2,
                         bool pos1_smooth = true, bool pos2_smooth = true);
    static void DrawWire(QPainter &p, QColor col, QPointF pos1, QPointF pos2,
                         bool pos1_smooth = true, bool pos2_smooth = true);
    static void DrawWire(QPainter &p, QColor col, int pen_width, QPointF pos1,
                         QPointF pos2, bool pos1_smooth = true,
                         bool pos2_smooth = true);
    /// 核心绘制函数
    static void DrawPathKernel(QPainter &p, QPointF pos1, QPointF pos2,
                               bool pos1_smooth, bool pos2_smooth);

public:
    explicit Wire(QObject *parent = nullptr, Kernel::Wire *tar = nullptr,
                  Port *lp1 = nullptr, Port *lp2 = nullptr);

signals:
};

} // namespace UserInterface

#endif // USERINTERFACE_WIRE_H
