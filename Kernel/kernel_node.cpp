#include "kernel_node.h"
namespace Kernel {

Node::Node(QObject *parent, NodeGraph *pNM)
    : QObject(parent), parentNodeGraph(pNM) {
    isFinished = false;
}

NodeGraph *Node::GetParentNodeManager() {
    return parentNodeGraph;
}

void Node::AddInputPort(PortDataType dt, QString n, bool hasDefault) {
    Port *npb = new Port(this, this, PortType::Input, dt, n, hasDefault);
    this->InputPorts.push_back(npb);
}

void Node::AddOutputPort(PortDataType dt, QString n) {
    Port *npb = new Port(this, this, PortType::Output, dt, n);
    this->OutputPorts.push_back(npb);
}

void Node::AddParamPort(PortDataType dt, QString n, bool hasDefault) {
    Port *npb = new Port(this, this, PortType::Param, dt, n, hasDefault);
    this->ParamPorts.push_back(npb);
}

void Node::AddComputeShaderFromPath(QString path) {
    QOpenGLShader *shader;
    QOpenGLShaderProgram *shaderProgram;
    shader = new QOpenGLShader(QOpenGLShader::Compute);
    shader->compileSourceFile(path);
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->create();
    shaderProgram->addShader(shader);
    shaderProgram->link();
    this->shaderPrograms.push_back(shaderProgram);
    this->shaders.push_back(shader);
}

void Node::ClearShaders() {
    for (int i = 0; i < shaderPrograms.size(); i++) {
        delete shaderPrograms[i];
    }
    for (int i = 0; i < shaders.size(); i++) {
        delete shaders[i];
    }
    shaderPrograms.clear();
    shaders.clear();
}

void Node::Allocate(QOpenGLFunctions_4_5_Core &f) {
    // 准备缓存
    for (auto &i : OutputPorts) {
        i->AllocateBuffer(f);
    }
    for (auto &i : InputPorts) {
        i->AllocateBuffer(f);
    }
}

void Node::Release(QOpenGLFunctions_4_5_Core &f) {
    // 释放所有缓存
    for (auto &i : OutputPorts) {
        i->DeleteBuffer(f);
    }
    //    for (auto &i : ParamPorts) {
    //        i->DeleteBuffer(f);
    //    }
}

bool Node::RunNode(QOpenGLFunctions_4_5_Core &f) {
    // 将所有输出接口可用状态置于否
    for (int i = 0; i < OutputPorts.length(); i++) {
        OutputPorts[i]->isAvailable = false;
    }
    // 将所有输入接口/参数接口的可用状态更新一遍
    for (auto &i : InputPorts) {
        i->UpdateAvailableState();
    }
    for (auto &i : ParamPorts) {
        i->UpdateAvailableState();
    }
    // 检查所有输入与参数节点是否一定需要输入/可以有默认值，并且检查所有输入值是否准备完毕
    for (int i = 0; i < InputPorts.length(); i++) {
        // 检查是否有必需连接的节点没有链接
        if (!(InputPorts[i]->hasDefaultValue) && !(InputPorts[i]->isLinked())) {
            qDebug() << "Error: Prerequisites for node computing are not met.";
            return false;
        }
        // 检查所有输入/参数节点是否准备就绪，若不就绪，那么运行前置节点
        if (!(InputPorts[i]->isAvailable)) {
            Node *preNode = InputPorts[i]->LinkedPorts[0]->GetParentNode();
            preNode->RunNode(f);
        }
    }
    for (int i = 0; i < ParamPorts.length(); i++) {
        // 检查是否有必需连接的节点没有链接
        if (!ParamPorts[i]->hasDefaultValue && !ParamPorts[i]->isLinked()) {
            qDebug() << "Error: Prerequisites for node computing are not met.";
            return false;
        }
        // 检查所有输入/参数节点是否准备就绪，若不就绪，那么运行前置节点
        // 输入/参数节点在有默认值的且未连接的情况下是一定就绪的，当输入/参数节点在有链接的情况下，是否就绪取决于链接的节点
        if (!(ParamPorts[i]->isAvailable)) {
            Node *preNode = ParamPorts[i]->LinkedPorts[0]->GetParentNode();
            preNode->RunNode(f);
        }
    }
    CalculateNode(f);
    for (int i = 0; i < OutputPorts.length(); i++) {
        OutputPorts[i]->isAvailable = true;
        for (int i = 0; i < OutputPorts[i]->LinkedPorts.length(); i++) {
            OutputPorts[i]->LinkedPorts[i]->UpdateAvailableState();
        }
    }
    isFinished = true;
    return true;
}

} // namespace kernel
