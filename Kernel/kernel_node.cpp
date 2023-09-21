#include "kernel_node.h"

namespace Kernel {

Node::Node(QObject *parent, NodeGraph *pNM)
    : QObject(parent), parentNodeGraph(pNM) {
    isFinished = false;
    name = "";
}

// Node::~Node() {
//     // 先清除所有接口
//     for (auto &i : InputPorts) {
//         delete i;
//     }
//     InputPorts.clear();
//     for (auto &i : OutputPorts) {
//         delete i;
//     }
//     OutputPorts.clear();
//     for (auto &i : ParamPorts) {
//         delete i;
//     }
//     ParamPorts.clear();
//     // 然后清除非接口参数
//     for (auto &i : nonPortParams) {
//         delete i;
//     }
//     nonPortParams.clear();
//     // 清除shder和program
//     ClearShaders();
//     // 置空
//     parentNodeGraph = nullptr;
// }

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
    qDebug() << name << "节点 第" << shaderPrograms.size()
             << "个着色器编译结果：";
    qDebug() << shaderProgram->log();
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
        i->AllocateOrUpdateData(f);
    }
    for (auto &i : InputPorts) {
        i->AllocateOrUpdateData(f);
    }
    for (auto &i : ParamPorts) {
        i->AllocateOrUpdateData(f);
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
            //            qDebug() << "Error: Prerequisites for node computing
            //            are not met.";
            return false;
        }
        // 检查所有输入/参数节点是否准备就绪，若不就绪，那么运行前置节点
        if (!(InputPorts[i]->isAvailable)) {
            Node *preNode = InputPorts[i]->LinkedPorts[0]->GetParentNode();
            preNode->InitGL(f);
            preNode->Allocate(f);
            preNode->RunNode(f);
            preNode->Choose(f);
            // 本节点的输入接口重新更新
            InputPorts[i]->AllocateOrUpdateData(f);
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
            // 本节点的输入接口重新更新
            ParamPorts[i]->AllocateOrUpdateData(f);
        }
    }
    CalculateNode(f);
    for (auto &i : OutputPorts) {
        i->isAvailable = true;
        for (auto &j : i->LinkedPorts) {
            j->UpdateAvailableState();
        }
    }
    isFinished = true;
    return true;
}

bool Node::OccurChangeOnPort(Port *tar) {
    //    qDebug() << "参数接口改变";
    bool flag = false;
    // 更新改动的接口的状态
    for (auto &i : InputPorts) {
        if (i == tar) {
            i->UpdateAvailableState();
            //            i->DeleteBuffer(*globalgl::thisContext);
            flag = true;
            break;
        }
    }
    for (auto &i : ParamPorts) {
        if (i == tar) {
            i->UpdateAvailableState();
            //            i->DeleteBuffer(*globalgl::thisContext);
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
    //    qDebug() << "非接口参数改变";
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
    int num = 0;
    for (auto &i : OutputPorts) {
        num++;
        i->isAvailable = false;
        int num_1 = 0;
        for (auto &j : i->LinkedPorts) {
            num_1++;
            j->GetParentNode()->OccurChangeOnPort(j);
        }
        //        qDebug() << "第" << num << "个输出接口，共" << num_1
        //                 << "个与之相连的接口被更新";
    }
    //    qDebug() << "共" << num << "个输出接口 ";
    return true;
}

void Node::SetPixel(QOpenGLFunctions_4_5_Core &f, unsigned int Image2D, int x,
                    int y, float value) {
    // 先创立shader
    QOpenGLShader *shader;
    QOpenGLShaderProgram *shaderProgram;
    shader = new QOpenGLShader(QOpenGLShader::Compute);
    shader->compileSourceFile(":/ComputeShaders/SetPixel.comp");
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->create();
    shaderProgram->addShader(shader);
    shaderProgram->link();

    f.glBindImageTexture(0, Image2D, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

    shaderProgram->bind();
    shaderProgram->setUniformValue("data", value);

    shaderProgram->setUniformValue("index_x", x);
    shaderProgram->setUniformValue("index_y", y);

    f.glDispatchCompute(1, 1, 1);

    delete shaderProgram;
    delete shader;

    return;
}

void Node::SetImage2D(QOpenGLFunctions_4_5_Core &f, unsigned int Image2D,
                      float *array, int count, int interval) {
    if (count % interval != 0) {
        qDebug() << "函数Set Image 2D"
                 << "参数count不能整除interval";
        return;
    }

    // 先创立shader
    QOpenGLShader *shader;
    QOpenGLShaderProgram *shaderProgram;
    shader = new QOpenGLShader(QOpenGLShader::Compute);
    shader->compileSourceFile(":/ComputeShaders/SetPixel.comp");
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->create();
    shaderProgram->addShader(shader);
    shaderProgram->link();

    f.glBindImageTexture(0, Image2D, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

    for (int i = 0; i < count / interval; ++i) {
        for (int j = 0; j < interval; ++j) {
            shaderProgram->bind();

            float value = array[i * interval + j];

            shaderProgram->setUniformValue("data", value);

            shaderProgram->setUniformValue("index_x", i);
            shaderProgram->setUniformValue("index_y", j);

            f.glDispatchCompute(1, 1, 1);
        }
    }

    delete shaderProgram;
    delete shader;

    return;
}

} // namespace Kernel
