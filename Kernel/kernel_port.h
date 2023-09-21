#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H

#include <QDebug>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QString>
#include <QVector>

#include "Global/globalinfo.h"
#include "kernel_element_forwarddec.cpp"

// 前置声明
namespace UserInterface {
class Port;
}

namespace Kernel {

// 前置声明
ForwardDeclarationWithoutPort;

/// 枚举接口的种类：输入、参数配置输入、输出
enum class PortType {
    /// 输入
    Input,
    /// 输出
    Output,
    /// 参数
    Param
};

/// 枚举接口的数据类型：二维float、二维RGBA8、单个float数、点云等
enum class PortDataType {
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

/// 接口————后端抽象计算层
class Port : public QObject {
    Q_OBJECT
protected:
    /// 指向该接口所在的节点（父节点），只能使用指针
    Node *parentNode;

public:
    /// 该节点指向的ui节点
    UserInterface::Port *targetUIPort;

public:
    /// 获取该接口所在的节点（父节点）
    Node *GetParentNode();

protected:
    /// 该接口的类型
    PortType type;
    /// 该接口传递的数据类型
    PortDataType dataType;

public:
    /// 一个字符串名字
    QString name;

public:
    /// 是否有默认值(是否不一定需要输入)
    const bool hasDefaultValue;

protected:
    /// 连接的buffer（高度场、图像、点云），是GL缓存指针
    unsigned int ConBuffer;
    /// 连接的单个浮点数的值（并非地址，因为无必要）
    float ConFloat;
    /// 默认值 buffer
    unsigned int defaultBuffer;
    /// 默认值 float
    float defaultFloat;

public:
    /// 浮点数是否有对应的范围
    bool isRanged;
    /// 最小值
    float rangefloat_l;
    /// 最大值
    float rangefloat_r;

public:
    /// 连接的接口数组，如果是输入接口，那么该数组只有一个值，否则可能有一系列值
    QVector<Port *> LinkedPorts;
    /// 是否可用：输入节点就是前置输出节点的该值，输出节点就是该节点指向的数据是否计算完毕
    bool isAvailable;
    /// 是否创建了显存缓存
    bool isAllocated;
    /// 创建显存缓存（高度场与贴图）
    void AllocateOrUpdateData(QOpenGLFunctions_4_5_Core &f);
    /// 创建显存缓存（点云）
    void AllocateOrUpdateData(QOpenGLFunctions_4_5_Core &f, long long size);
    /// 删除显存缓存
    void DeleteBuffer(QOpenGLFunctions_4_5_Core &f);

public:
    /// 从其他接口中复制信息
    void CopyFrom(QOpenGLFunctions_4_5_Core &f, Port *tar);

    /// 获取接口类型
    PortType GetType();
    /// 设置/切换接口数据类型
    void SetPortDataType(PortDataType t);
    /// 获取接口数据类型
    PortDataType GetDataType();

    /// 是否连接了某个接口
    bool isLinkedWith(Port *tar);
    /// 试图从连线获取连接的数据（如果是输入或参数节点），返回值表示是否成功
    bool TryGetDataFromWire();
    /// 获得连接的数据
    bool GetData(unsigned int &buffer, float &value);
    /// 获得连接的buffer
    unsigned int GetBufferData();
    /// 获得链接的float
    float GetFloatData();
    /// 获得连接的buffer
    unsigned int ForceGetBufferData();
    /// 获得链接的float
    float ForceGetFloatData();
    /// 设置数据
    void SetData(unsigned int buffer);
    /// 设置数据
    void SetData(float value);
    /// 判断是否连接
    bool isLinked();
    /// (弃用)连接函数，返回值表示连接是否成功
    bool Link(Port *targetPort);
    /// 根据传入数组更新Link信息
    void UpdateLinkInfo(QVector<Wire *> &wires);
    /// 更新是否可用信息
    void UpdateAvailableState();

    /// 设置默认值1
    void SetDefaultFloatData(float data);
    /// 设置默认值2
    void SetDefaultBufferData(unsigned int data);
    /// 获取默认值1
    float GetDefaultFloatData();
    /// 获取默认值2
    unsigned int GetDefaultBufferData();

public:
    //    Port(QObject *parent = nullptr, Node *pN = nullptr,
    //         PortType t = PortType::Input, PortDataType dt =
    //         PortDataType::Float2D, QString n = "未命名", bool hD =
    //         false);
    Port(QObject *parent, Node *pN = nullptr, PortType t = PortType::Input,
         PortDataType dt = PortDataType::Float2D, QString n = "未命名",
         bool hD = false, float defaultData = 0, bool isR = false,
         float R_l = 0, float R_r = 0);
    // 析构函数
    //    ~Port();

signals:
};
} // namespace Kernel
#endif // KERNEL_PORT_H
