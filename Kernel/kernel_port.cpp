#include "kernel_port.h"

#include "kernel_wire.h"

#define glCreateHeightField(glContext, data)                   \
    {                                                          \
        glContext.glGenTextures(1, &data);                     \
        glContext.glBindTexture(GL_TEXTURE_2D, data);          \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, \
                                 globalinfo::TerrainGrid,      \
                                 globalinfo::TerrainGrid);     \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);             \
    }

#define glCreateTexture(glContext, data)                                 \
    {                                                                    \
        glContext.glGenTextures(1, &data);                               \
        glContext.glBindTexture(GL_TEXTURE_2D, data);                    \
        glContext.glTexImage2D(                                          \
            GL_TEXTURE_2D, 0, GL_RGB, globalinfo::TerrainGrid,           \
            globalinfo::TerrainGrid, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); \
        glContext.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  \
                                  GL_LINEAR);                            \
        glContext.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  \
                                  GL_LINEAR);                            \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);                       \
    }

#define glCreatePointCloud(glContext, data, size)                        \
    {                                                                    \
        glContext.glGenTextures(1, &data);                               \
        glContext.glBindTexture(GL_TEXTURE_2D, data);                    \
        glContext.glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, size, 1); \
        glContext.glBindTexture(GL_TEXTURE_2D, 0);                       \
    }

namespace kernel {

Port::Port(QObject *parent, Node *pN, PortType t, PortDataType dt, QString n,
           bool hD)
    : QObject(parent), type(t), dataType(dt), name(n), hasDefaultValue(hD) {
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
    // 如果是输入/参数节点，那么将数据和相连的输出节点匹配或使用默认值
    if (type != PortType::Output) {
        if (!isLinked() && hasDefaultValue)
            return;

        else if (!isLinked() && !hasDefaultValue) {
            qDebug()
                << "ERROR: This node " + name +
                       " does not have default value and is not connected.";
            return;
        } else {
            this->ConFloat = this->LinkedPorts[0]->ForceGetFloatData();
            this->ConBuffer = this->LinkedPorts[0]->ForceGetBufferData();

            isAllocated = true;

            return;
        }
        //        qDebug() << "ERROR: input or param port need not allocate
        //        buffer!";
    }
    if (isAllocated) {
        qDebug() << "ERROR: Buffer has been allocated, it is cannot be "
                    "allocated again!";
        return;
    }
    //    QOpenGLFunctions_4_5_Core f;
    //    f.initializeOpenGLFunctions();
    // 如果是二维高度场的话
    if (this->dataType == PortDataType::Float2D) {
        glCreateHeightField(f, ConBuffer);
    } else if (this->dataType == PortDataType::RGBA2D) {
        glCreateTexture(f, ConBuffer);
    } else if (this->dataType == PortDataType::Float) {
        ConBuffer = 0;
        ConFloat = 0;
    }
    // 如果创建点云的话
    else {
        qDebug() << "ERROR: You should give an argument more to descibe the "
                    "number of pointers in the point cloud!";
        return;
    }

    isAllocated = true;
}

void Port::AllocateBuffer(QOpenGLFunctions_4_5_Core &f, long long size) {
    // 如果是输入/参数节点，那么将数据和相连的输出节点匹配或使用默认值
    if (type != PortType::Output) {
        if (!isLinked() && hasDefaultValue)
            return;

        else if (!isLinked() && !hasDefaultValue) {
            qDebug()
                << "ERROR: This node " + name +
                       " does not have default value and is not connected.";
            return;
        } else {
            this->ConFloat = this->LinkedPorts[0]->ForceGetFloatData();
            this->ConBuffer = this->LinkedPorts[0]->ForceGetBufferData();

            isAllocated = true;

            return;
        }
        //        qDebug() << "ERROR: input or param port need not allocate
        //        buffer!";
    }
    if (isAllocated) {
        qDebug() << "ERROR: Buffer has been allocated, it is cannot be "
                    "allocated again!";
        return;
    }
    // 如果是二维点云的话
    if (this->dataType == PortDataType::PointCloud2D) {
        glCreatePointCloud(f, ConBuffer, size);
    }
    // 如果是三维点云的话
    else if (this->dataType == PortDataType::PointCloud3D) {
        glCreatePointCloud(f, ConBuffer, size);
    }
    // 如果创建非点云的话
    else {
        qDebug() << "ERROR: Create a non-point-cloud buffer should use no "
                    "argument to describe size!";
        return;
    }

    isAllocated = true;
}

void Port::DeleteBuffer(QOpenGLFunctions_4_5_Core &f) {
    if (!isAllocated) {
        qDebug() << "ERROR: Try deleting buffer before the buffer created!";
        return;
    }
    // 如果是二维高度场、二维点云、三维点云的话
    if (this->dataType != PortDataType::Float) {
        f.glDeleteTextures(1, &ConBuffer);
    } else {
        ConBuffer = 0;
        ConFloat = 0;
    }
    isAllocated = false;
}

void Port::CopyFrom(QOpenGLFunctions_4_5_Core &f, Port *tar) {
    if (isAllocated) DeleteBuffer(f);
    this->type = tar->GetType();
    this->dataType = tar->GetDataType();
    this->ConBuffer = tar->ForceGetBufferData();
    this->ConFloat = tar->ForceGetFloatData();
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

PortDataType Port::GetDataType() {
    return this->dataType;
}

bool Port::isLinkedWith(Port *tar) {
    for (int i = 0; i < LinkedPorts.length(); i++) {
        if (LinkedPorts[i] == tar) return true;
    }
    return false;
}

bool Port::TryGetDataFromWire() {
    if (this->type == PortType::Output) {
        qDebug() << "This port need not to be tried getting data from wire "
                    "because it is an output port.";
        return false;
    } else {
        if (!isLinked()) {
            return false;
        } else {
            this->ConBuffer = LinkedPorts[0]->GetBufferData();
            this->ConFloat = LinkedPorts[0]->GetFloatData();
            return true;
        }
    }
}

unsigned int Port::ForceGetBufferData() {
    return ConBuffer;
}

float Port::ForceGetFloatData() {
    return ConFloat;
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

void Port::UpdateLinkInfo(QVector<Wire *> &wires) {
    this->LinkedPorts.clear();
    // 如果是输入节点
    if (this->type == PortType::Input || this->type == PortType::Param) {
        for (int i = 0; i < wires.length(); i++) {
            if (wires[i]->GetOutput() == this) {
                this->LinkedPorts.push_back(wires[i]->GetInput());
            }
        }
    }
    // 如果是输出节点
    else {
        for (int i = 0; i < wires.length(); i++) {
            if (wires[i]->GetOutput() == this) {
                this->LinkedPorts.push_back(wires[i]->GetInput());
            }
        }
    }
}

void Port::UpdateAvailableState() {
    if (type == PortType::Input || type == PortType::Param) {
        if (isLinked()) {
            this->isAvailable = LinkedPorts[0]->isAvailable;
        } else {
            this->isAvailable = this->hasDefaultValue;
        }
    } else {
        qDebug() << "Warning: Output port " + name +
                        " should not update its available state.";
    }
}

} // namespace kernel
