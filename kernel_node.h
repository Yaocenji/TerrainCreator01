#ifndef KERNEL_NODE_H
#define KERNEL_NODE_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QVector>

#include "kernel_port.h"

namespace kernel {
/// 前置声明：父类
class NodeGraph;

/// 节点————后端抽象计算层
class Node : public QObject {
    Q_OBJECT
protected:
    /// 所属的节点图层，在任何情况下都不应该为null
    NodeGraph *parentNodeManager;

public:
    /// 获取所属的节点图层
    NodeGraph *GetParentNodeManager();

public:
    /// 输入节点数组
    QVector<Port *> InputPorts;
    /// 输出节点数组
    QVector<Port *> OutputPorts;
    /// 参数节点数组
    QVector<Port *> ParamPorts;
    /// 计算着色器数组
    QVector<QOpenGLShaderProgram *> shaderPrograms;

public:
    /// 添加输入节点
    void AddInputPort(PortDataType dt, QString n);
    /// 添加输出节点
    void AddOutputPort(PortDataType dt, QString n);
    /// 添加参数节点
    void AddParamPort(PortDataType dt, QString n);

public:
    /// 初始化GL
    virtual void InitGL(QOpenGLFunctions_4_5_Core &f) = 0;
    /// 运行函数
    virtual void RunNode(QOpenGLFunctions_4_5_Core &f) = 0;
    /// 选择该节点
    virtual void Choose(QOpenGLFunctions_4_5_Core &f) = 0;

public:
    explicit Node(QObject *parent = nullptr, NodeGraph *pNM = nullptr);

signals:
};

} // namespace kernel

#endif // KERNEL_NODE_H
