#include "kernel_port.h"
namespace kernel {

Port::Port(QObject *parent, Node *pN, PortType t, PortDataType dt, QString n)
    : QObject(parent), type(t), dataType(dt), name(n) {
    parentNode = pN;
    ConBuffer = 0;
    ConFloat = 0;
    LinkedPorts.resize(0);
    isAvailable = false;
    isAllocated = false;
}

Node *Port::GetParentNode() {
    return parentNode;
}

void Port::AllocateBuffer(QOpenGLFunctions_4_5_Core &f) {
    if (isAllocated) {
        qDebug() << "ERROR: Buffer has been allocated, it is cannot be "
                    "allocated again!";
        return;
    }
    //    QOpenGLFunctions_4_5_Core f;
    //    f.initializeOpenGLFunctions();
    // 如果是二维高度场、二维点云、三维点云的话
    if (this->dataType != PortDataType::RGBA2D &&
        this->dataType != PortDataType::Float) {
        f.glGenTextures(1, &ConBuffer);
        f.glBindTexture(GL_TEXTURE_2D, ConBuffer);
        f.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, globalinfo::TerrainGrid,
                         globalinfo::TerrainGrid);
    } else if (this->dataType == PortDataType::RGBA2D) {
        f.glGenTextures(1, &ConBuffer);
        f.glBindTexture(GL_TEXTURE_2D, ConBuffer);
        f.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, globalinfo::TerrainGrid,
                       globalinfo::TerrainGrid, 0, GL_RGB, GL_UNSIGNED_BYTE,
                       NULL);
        f.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        f.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        f.glBindTexture(GL_TEXTURE_2D, 0);
    } else if (this->dataType == PortDataType::Float) {
        ConBuffer = 0;
        ConFloat = 0;
    }
    isAllocated = true;
}

void Port::DeleteBuffer(QOpenGLFunctions_4_5_Core &f) {
    if (!isAllocated) {
        qDebug() << "ERROR: Try deleting buffer before the buffer created!";
        return;
    }
    //    QOpenGLFunctions_4_5_Core f;
    //    f.initializeOpenGLFunctions();
    // 如果是二维高度场、二维点云、三维点云的话
    if (this->dataType != PortDataType::Float) {
        f.glDeleteTextures(1, &ConBuffer);
    } else {
        ConBuffer = 0;
        ConFloat = 0;
    }
    isAllocated = false;
}

PortType Port::GetType() {
    return type;
}

void Port::SetPortDataType(PortDataType t) {
    this->dataType = t;
    if (t != PortDataType::Float) {
        ConFloat = 0;
    } else {
        ConBuffer = 0;
    }
}

PortDataType Port::GetPortDataType() {
    return this->dataType;
}

unsigned int Port::GetBufferData() {
    if (dataType == PortDataType::Float) {
        qDebug() << "It is wrong to try getting wrong type data.";
        return 0;
    }
    return ConBuffer;
}
float Port::GetFloatData() {
    if (dataType != PortDataType::Float) {
        qDebug() << "It is wrong to try getting wrong type data.";
        return 0.0;
    }
    return ConFloat;
}

void Port::GetData(unsigned int &buffer, float &value) {
    if (dataType == PortDataType::Float) {
        buffer = 0;
        value = ConFloat;
    } else {
        buffer = ConBuffer;
        value = 0;
    }
}

void Port::SetData(unsigned int buffer) {
    // 如果传递了buffer而数据类型是float，报错
    if (dataType == PortDataType::Float) {
        qDebug() << "ERROR: Try to set a buffer-typed data to a "
                    "float-value-typed port.";
        return;
    }
    // 如果对应，那么改变data
    this->ConBuffer = buffer;
    this->ConFloat = 0;
}

void Port::SetData(float value) {
    // 如果传递了buffer而数据类型是float，报错
    if (dataType != PortDataType::Float) {
        qDebug() << "ERROR: Try to set a buffer-typed data to a "
                    "float-value-typed port.";
        return;
    }
    // 如果对应，那么改变data
    this->ConBuffer = 0;
    this->ConFloat = value;
}

bool Port::isLinked() {
    if (LinkedPorts.empty())
        return false;
    else
        return true;
}

bool Port::Link(Port *targetPort) {
    // 如果该节点是输出而对方节点是输入或参数
    if (this->type == PortType::Output &&
        targetPort->type != PortType::Output) {
        // 判断是否和该节点已连接
        /// 本节点链接列表是否含有对方节点
        bool ifThisPortsHasTar = false;
        for (int i = 0; i < this->LinkedPorts.length(); i++) {
            if (LinkedPorts[i] == targetPort) {
                ifThisPortsHasTar = true;
                break;
            }
        }
        // 连接成立：已经连接过了
        if (ifThisPortsHasTar) {
            qDebug() << "WARNING: They has Linked.";
            return false;
        }

        // 不成立：没有链接过
        // 判断数据类型是否相符
        // 数据类型相符
        if (this->dataType == targetPort->dataType) {
            // 对方节点调用Link函数
            targetPort->Link(this);
            LinkedPorts.push_back(targetPort);
            return true;
        }
        // 不相符
        qDebug() << "WARNING: The data types are different, so "
                    "they cannot be linked.";
        return false;

    }
    // 该节点是输入或参数，对方节点是输出节点
    else if (this->type != PortType::Output &&
             targetPort->type == PortType::Output) {
        // 判断是否和该节点已连接
        // 已经连接
        if (!LinkedPorts.empty() && LinkedPorts[0] == targetPort) {
            qDebug() << "WARNING: They has Linked.";
            return false;
        }
        // 没有链接过
        // 判断数据类型
        if (this->dataType == targetPort->dataType) {
            // 输入或参数节点只能连一个，所以直接clear再添加
            targetPort->Link(this);
            LinkedPorts.clear();
            LinkedPorts.push_back(targetPort);
            return true;
        }
        // 数据类型不能对应
        qDebug() << "WARNING: The data types are different, so "
                    "they cannot be linked.";
        return false;
    }
    // 这两个节点类型不对应，无法连接
    else {
        qDebug() << "ERROR: The port types are not corresponding, so "
                    "they cannot be linked.";
        return false;
    }
}

} // namespace kernel
