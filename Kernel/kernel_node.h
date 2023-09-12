#ifndef KERNEL_NODE_H
#define KERNEL_NODE_H

#include <QColor>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QVector>

#include "Global/globalfunc.h"
#include "kernel_port.h"

namespace Kernel {
/// 前置声明
class NodeGraph;

/// 非接口参数的类型
enum class NonPortParamType {
    /// 任意浮点
    Float,
    /// 范围浮点
    RangeFloat,
    /// 任意整数
    Int,
    /// 范围整数
    RangeInt,
    /// 枚举（数组）
    Enum,
    /// 颜色（4通道8位）
    Color,
};

/// 非接口参数
class NonPortParam {
public:
    /// 该参数的类型
    NonPortParamType type;
    /// 该参数的显示名字
    QString name;

    // 该参数的所有可能数据
    float data_float;

    float data_rangefloat;
    float rangefloat_l;
    float rangefloat_r;

    int data_int;

    int data_rangeint;
    int rangeint_l;
    int rangeint_r;

    int data_enum;
    QVector<QString> enum_name;

    QColor data_color;

    /// 方法：获取数据
    int GetData(float &f, int &i, int &e, QColor &c) {
        if (type == NonPortParamType::Float) {
            f = data_float;
            return 0;
        } else if (type == NonPortParamType::RangeFloat) {
            f = data_rangefloat;
            return 0;
        } else if (type == NonPortParamType::Int) {
            i = data_int;
            return 1;
        } else if (type == NonPortParamType::RangeInt) {
            i = data_rangeint;
            return 1;
        } else if (type == NonPortParamType::Enum) {
            e = data_enum;
            return 2;
        } else if (type == NonPortParamType::Color) {
            c = data_color;
            return 3;
        }
        return -1;
    }

    /// 构造函数
    NonPortParam(NonPortParamType ty, QString na) : type(ty), name(na) {
        data_float = 0;
        data_rangefloat = 0;
        rangefloat_l = 0;
        rangefloat_r = 0;

        data_int = 0;

        data_rangeint = 0;
        rangeint_l = 0;
        rangeint_r = 0;

        data_enum = 0;
        enum_name = QVector<QString>();

        data_color = QColor();
    }

    // 一系列初始化函数
    bool InitData(int value) {
        if (type != NonPortParamType::Int) {
            qDebug() << "NonPortParam设置数据时，类型不匹配！";
            return false;
        }
        data_int = value;
        return true;
    }
    bool InitData(int value, int min, int max) {
        if (type != NonPortParamType::RangeInt) {
            qDebug() << "NonPortParam设置数据时，类型不匹配！";
            return false;
        }
        data_rangeint = value;
        rangeint_l = min;
        rangeint_r = max;
        return true;
    }
    bool InitData(float value) {
        if (type != NonPortParamType::Float) {
            qDebug() << "NonPortParam设置数据时，类型不匹配！";
            return false;
        }
        data_float = value;
        return true;
    }
    bool InitData(float value, float min, float max) {
        if (type != NonPortParamType::RangeFloat) {
            qDebug() << "NonPortParam设置数据时，类型不匹配！";
            return false;
        }
        data_rangefloat = value;
        rangefloat_l = min;
        rangefloat_r = max;
        return true;
    }
    bool InitData(int value, QVector<QString> en_na) {
        if (type != NonPortParamType::Enum) {
            qDebug() << "NonPortParam设置数据时，类型不匹配！";
            return false;
        }
        data_enum = value;
        enum_name = en_na;
        return true;
    }
    bool InitData(QColor value) {
        if (type != NonPortParamType::Color) {
            qDebug() << "NonPortParam设置数据时，类型不匹配！";
            return false;
        }
        data_color = value;
        return true;
    }

    // 一系列设置函数
    bool SetData(int value) {
        if (type == NonPortParamType::Int) {
            data_int = value;
            return true;
        } else if (type == NonPortParamType::RangeInt) {
            data_rangeint = clamp<int>(rangeint_l, rangeint_r, value);
            return true;
        } else if (type == NonPortParamType::Enum) {
            if (value >= 0 && value < enum_name.size()) {
                data_enum = value;
                return true;
            } else {
                qDebug() << "NonPortParam设置值失败：枚举类数据越界";
                return false;
            }
        } else {
            qDebug()
                << "NonPortParam设置值失败：传入参数与要设置的数据类型不对应";
            return false;
        }
    }
    bool SetData(float value) {
        if (type == NonPortParamType::Float) {
            data_float = value;
            return true;
        } else if (type == NonPortParamType::RangeFloat) {
            data_rangefloat = clamp<float>(rangefloat_l, rangefloat_r, value);
            return true;
        } else {
            qDebug()
                << "NonPortParam设置值失败：传入参数与要设置的数据类型不对应";
            return false;
        }
    }
    bool SetData(QColor value) {
        if (type == NonPortParamType::Color) {
            data_color = value;
            return true;
        } else {
            qDebug()
                << "NonPortParam设置值失败：传入参数与要设置的数据类型不对应";
            return false;
        }
    }
};

/// 节点————后端抽象计算层
class Node : public QObject {
    Q_OBJECT
public:
    /// 名字
    QString name;

protected:
    /// 所属的节点图层，在任何情况下都不应该为null
    NodeGraph *parentNodeGraph;

public:
    /// 获取所属的节点图层
    NodeGraph *GetParentNodeManager();

public:
    /// 是否计算完成
    bool isFinished;

public:
    /// 输入节点数组
    QVector<Port *> InputPorts;
    /// 输出节点数组
    QVector<Port *> OutputPorts;
    /// 参数节点数组
    QVector<Port *> ParamPorts;

    /// 非接口参数表：这些参数并不开放接口，但是在inspector中可以调整
    QVector<NonPortParam *> nonPortParams;

    /// 着色器存储
    QVector<QOpenGLShader *> shaders;
    /// 计算着色器程序数组
    QVector<QOpenGLShaderProgram *> shaderPrograms;

public:
    /// 显示颜色
    QColor color;

public:
    /// 添加输入节点
    void AddInputPort(PortDataType dt, QString n, bool hasDefault,
                      float defaultData = 0);
    /// 添加输出节点
    void AddOutputPort(PortDataType dt, QString n);
    /// 添加参数节点
    void AddParamPort(PortDataType dt, QString n, bool hasDefault,
                      float defaultData = 0);
    /// 添加非接口参数
    void AddNonPortParam(NonPortParamType type, QString name);
    /// 添加着色器
    void AddComputeShaderFromPath(QString path);
    /// 清空着色器
    void ClearShaders();

public:
    /// 初始化GL
    virtual void InitGL(QOpenGLFunctions_4_5_Core &f) = 0;
    /// 分配显存
    virtual void Allocate(QOpenGLFunctions_4_5_Core &f);
    /// 运行函数
    bool RunNode(QOpenGLFunctions_4_5_Core &f);
    /// 计算函数
    virtual void CalculateNode(QOpenGLFunctions_4_5_Core &f) = 0;
    /// 选择该节点
    virtual void Choose(QOpenGLFunctions_4_5_Core &f) = 0;
    /// 释放内存
    virtual void Release(QOpenGLFunctions_4_5_Core &f);

    /// 断连某个输入/参数接口
    bool OccurChangeOnPort(Port *tar);
    /// 某个非接口参数发生改变
    bool OccurChangeOnNonPortParam(NonPortParam *tar);

public:
    explicit Node(QObject *parent, NodeGraph *pNM = nullptr);
    //    ~Node();

signals:
};

} // namespace Kernel

#endif // KERNEL_NODE_H
