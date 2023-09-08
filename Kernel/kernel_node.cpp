#include "kernel_node.h"
namespace Kernel {

Node::Node(QObject *parent, NodeGraph *pNM)
    : QObject(parent), parentNodeGraph(pNM) {
    isFinished = false;
    name = "";
}

NodeGraph *Node::GetParentNodeManager() {
    return parentNodeGraph;
}

void Node::AddInputPort(PortDataType dt, QString n, bool hasDefault,
                        float defaultData) {
    Port *npb;
    if (hasDefault)
        npb = new Port(this, this, PortType::Input, dt, n, hasDefault);
    else {
        npb = new Port(this, this, PortType::Input, dt, n, hasDefault,
                       defaultData);
    }
    this->InputPorts.push_back(npb);
}

void Node::AddOutputPort(PortDataType dt, QString n) {
    Port *npb = new Port(this, this, PortType::Output, dt, n);
    this->OutputPorts.push_back(npb);
}

void Node::AddParamPort(PortDataType dt, QString n, bool hasDefault,
                        float defaultData) {
    Port *npb;
    if (!hasDefault)
        npb = new Port(this, this, PortType::Param, dt, n, hasDefault);
    else
        npb = new Port(this, this, PortType::Param, dt, n, hasDefault,
                       defaultData);
    this->ParamPorts.push_back(npb);
}

void Node::AddNonPortParam(NonPortParamType type, QString name) {
    NonPortParam *newParam = new NonPortParam(type, name);
    nonPortParams.push_back(newParam);
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
            preNode->InitGL(f);
            preNode->Allocate(f);
            preNode->RunNode(f);
            preNode->Choose(f);
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
            preNode->InitGL(f);
            preNode->Allocate(f);
            preNode->RunNode(f);
            preNode->Choose(f);
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

bool Node::OccurChangeOnPort(Port *tar) {
    bool flag = false;
    // 更新改动的接口的状态
    for (auto &i : InputPorts) {
        if (i == tar) {
            i->UpdateAvailableState();
            flag = true;
            break;
        }
    }
    for (auto &i : ParamPorts) {
        if (i == tar) {
            i->UpdateAvailableState();
            flag = true;
            break;
        }
    }

    if (!flag) {
        qDebug() << "error：接口发生改动，调用OccurChangeOnPort时，传入的接口指"
                    "针不包含在该节点内";
        return false;
    }
    // 更新节点的完成状态
    this->isFinished = false;
    // 更新所有的输出接口状态与和输出接口相连的节点的状态（递归调用该函数）
    for (auto &i : OutputPorts) {
        i->isAvailable = false;
        for (auto &j : i->LinkedPorts) {
            j->GetParentNode()->OccurChangeOnPort(j);
        }
    }
    return true;
}

bool Node::OccurChangeOnNonPortParam(NonPortParam *tar) {
    bool flag = false;
    // 更新改动的接口的状态
    for (auto &i : nonPortParams) {
        if (i == tar) {
            flag = true;
            break;
        }
    }

    if (!flag) {
        qDebug() << "error：非接口参数发生改动，调用OccurChangeOnNonPortParam时"
                    "，传入的接口指针不包含在该节点内";
        return false;
    }
    // 更新节点的完成状态
    this->isFinished = false;
    // 更新所有的输出接口状态与和输出接口相连的节点的状态（递归调用该函数）
    for (auto &i : OutputPorts) {
        i->isAvailable = false;
        for (auto &j : i->LinkedPorts) {
            j->GetParentNode()->OccurChangeOnPort(j);
        }
    }
    return true;
}

} // namespace Kernel
