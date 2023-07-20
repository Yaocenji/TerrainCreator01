#ifndef PORTOUTPUTBACK_H
#define PORTOUTPUTBACK_H

#include <QObject>

/// 枚举接口的种类：二维float、二维RGBA8、单个float数、点云等
enum class PortOutputType {
    /// 二维二维高度场
    Float2D,
    /// RGBA8图像（四通道八位整型）
    RGBA2D,
    /// 单个浮点数
    Float,
    /// 二位点云
    PointCloud2D,
    /// 三维点云
    PointCloud3D
};

/// 输出接口————后端抽象计算层
class PortOutputBack : public QObject {
    Q_OBJECT
protected:
    /// 该接口传递的数据类型
    PortOutputType type;

public:
    /// 连接的buffer（高度场、图像、点云），是GL缓存指针
    unsigned int ConBuffer;
    /// 连接的单个浮点数的值（并非地址，因为无必要）
    float ConFloat;

public:
    /// 设置/切换接口类型
    void SetPortType(PortOutputType t);
    /// 获取接口类型
    PortOutputType GetPortType();
    /// 获得连接的数据
    void GetData(unsigned int &buffer, float &value);

public:
    explicit PortOutputBack(QObject *parent = nullptr);

signals:
};

#endif // PORTOUTPUTBACK_H
