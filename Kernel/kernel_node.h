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

#include "kernel_port.h"

namespace Kernel {
/// 前置声明
class NodeGraph;

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

public:
    explicit Node(QObject *parent = nullptr, NodeGraph *pNM = nullptr);

signals:
};

} // namespace Kernel

#endif // KERNEL_NODE_H
