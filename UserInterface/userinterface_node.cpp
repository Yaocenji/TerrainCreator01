#include "userinterface_node.h"

#include "userinterface_nodegraph.h"

namespace UserInterface {

Node::Node(QObject *parent, Kernel::Node *tar, NodeGraph *parentNG, QPointF pos)
    : QObject(parent), targetNode(tar), parentNodeGraph(parentNG) {
    // 初始化名字
    name = targetNode->name;
    // 初始化节点尺寸
    rect.setTopLeft(pos);
    CalSetRectSize();
    // 添加接口
    // TODO
    for (int i = 0; i < tar->InputPorts.size(); i++) {
        Port *newPort =
            new Port(this, tar->InputPorts[i], this, PortType::Input, " ", i);
        InputPorts.push_back(newPort);
    }
    for (int i = 0; i < tar->OutputPorts.size(); i++) {
        Port *newPort =
            new Port(this, tar->OutputPorts[i], this, PortType::Output, " ", i);
        OutputPorts.push_back(newPort);
    }
    for (int i = 0; i < tar->ParamPorts.size(); i++) {
        Port *newPort =
            new Port(this, tar->ParamPorts[i], this, PortType::Param, " ", i);
        ParamPorts.push_back(newPort);
    }
}

QRectF Node::GetRectInfo() {
    return rect;
}

void Node::CalSetRectSize() {
    float input_column_height =
        globalui::input_column_found_size[0] +
        globalui::input_column_found_size[1] +
        targetNode->InputPorts.size() * globalui::port_interval_size;
    float output_column_height =
        globalui::output_column_found_size[0] +
        globalui::output_column_found_size[1] +
        targetNode->OutputPorts.size() * globalui::port_interval_size;
    float param_line_height =
        globalui::param_line_found_size[0] +
        globalui::param_line_found_size[1] +
        targetNode->ParamPorts.size() * globalui::port_interval_size;
    QVector2D size(param_line_height, input_column_height > output_column_height
                                          ? input_column_height
                                          : output_column_height);
    rect.setSize(QSizeF(size.x(), size.y()));
}

QColor Node::color() {
    return targetNode->color;
}

void Node::Draw(QPainter &p) {
    // 初始化画笔
    p.setPen(QPen(color(), globalui::pen_width));
    p.setBrush(QBrush(
        QColor(color().red() / 2, color().green() / 2, color().blue() / 2)));

    // 自动绘制圆角矩形
    p.drawRoundedRect(this->rect, globalui::node_angle_radius,
                      globalui::node_angle_radius);

    // 绘制接口
    for (auto &i : InputPorts) {
        i->Draw(p);
    }
    for (auto &i : OutputPorts) {
        i->Draw(p);
    }
    for (auto &i : ParamPorts) {
        i->Draw(p);
    }

    // 写名字
    p.setFont(globalui::node_name_font);
    p.drawText(rect.topLeft() + globalui::node_name_pos, name);
}

bool Node::ClickDetect(QPointF &pos, Node *&clickedNode, Port *&clickedPort) {
    for (auto &i : InputPorts) {
        if (i->ClickDetect(pos, clickedPort)) {
            clickedNode = this;
            return true;
        }
    }
    for (auto &i : OutputPorts) {
        if (i->ClickDetect(pos, clickedPort)) {
            clickedNode = this;
            return true;
        }
    }
    for (auto &i : ParamPorts) {
        if (i->ClickDetect(pos, clickedPort)) {
            clickedNode = this;
            return true;
        }
    }
    if (pos.x() <= rect.right() && pos.x() >= rect.left() &&
        pos.y() >= rect.top() && pos.y() <= rect.bottom()) {
        clickedNode = this;
        return true;
    } else {
        return false;
    }
}

void Node::Move(QPointF &delta) {
    rect.moveTo(rect.topLeft() + delta);
}

void Node::PortSuspensionUpdate(Port *tar) {
    for (auto &i : InputPorts) {
        i->PortSuspensionUpdate(tar);
    }
    for (auto &i : ParamPorts) {
        i->PortSuspensionUpdate(tar);
    }
    for (auto &i : OutputPorts) {
        i->PortSuspensionUpdate(tar);
    }
}

} // namespace UserInterface
