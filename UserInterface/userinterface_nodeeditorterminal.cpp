#include "userinterface_nodeeditorterminal.h"

namespace UserInterface {

NodeEditorTerminal::NodeEditorTerminal(QWidget *parent) : QWidget(parent) {
    mState = MouseState::None;
    oldMousePos = QPointF(0.0, 0.0);
}

void NodeEditorTerminal::SetTargetNodeGraph(NodeGraph *tar) {
    targetNodeGraph = tar;
}

void NodeEditorTerminal::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    // 反走样
    p.setRenderHint(QPainter::Antialiasing, true);

    // 设置摄像头变换
    if (targetNodeGraph != nullptr) {
        p.scale(1.0 / targetNodeGraph->GetCamSize(),
                1.0 / targetNodeGraph->GetCamSize());
        p.translate(-targetNodeGraph->GetCamPos());
        p.scale(3.0, 3.0);
    }

    // 设置画笔，绘制背景
    //
    QBrush br = QBrush(Option::background_color);
    br.setStyle(Qt::BrushStyle::SolidPattern);

    p.setPen(QColor(0, 0, 0, 0));
    p.setBrush(br);
    p.drawRect(targetNodeGraph->GetCamPos().x() / 3.0 - 7.0,
               targetNodeGraph->GetCamPos().y() / 3.0 - 7.0,
               this->width() * targetNodeGraph->GetCamSize() / 3.0 + 14.0,
               this->height() * targetNodeGraph->GetCamSize() / 3.0 + 14.0);

    br.setColor(Option::foreground_color);
    br.setStyle(Qt::BrushStyle::Dense7Pattern);
    p.setBrush(br);
    p.drawRect(targetNodeGraph->GetCamPos().x() / 3.0 - 7.0,
               targetNodeGraph->GetCamPos().y() / 3.0 - 7.0,
               this->width() * targetNodeGraph->GetCamSize() / 3.0 + 14.0,
               this->height() * targetNodeGraph->GetCamSize() / 3.0 + 14.0);

    // 绘制内容
    p.scale(1.0 / 3.0, 1.0 / 3.0);
    // 如果还没有要显示的目标图，那么返回
    if (targetNodeGraph == nullptr) {
        return;
    }
    // 否则绘制节点图
    else {
        targetNodeGraph->Draw(p);
    }
}

void NodeEditorTerminal::mousePressEvent(QMouseEvent *event) {
    // 真实的位置
    QPointF realPos = event->pos();
    realPos *= targetNodeGraph->GetCamSize();
    realPos += targetNodeGraph->GetCamPos();

    oldMousePos = realPos - targetNodeGraph->GetCamPos();
    // 如果是右键点击，那么：
    if (event->button() == Qt::RightButton) {
        // 如果目前是创建模式，那么取消创建
        if (mState == MouseState::CreateNode) {
            // TODO 取消创建模式
        }
        // 如果目前是空状态，那么进入抓住拖动模式
        if (mState == MouseState::None) {
            mState = MouseState::Grab;
            //            realPos -= targetNodeGraph->GetCamPos();
            //            oldMousePos = realPos;
        }

        update();
        return;
    }
    // 如果是鼠标左键
    // 创建一个新的节点
    if (mState == MouseState::CreateNode) {
        /// TODO
        //        node *n = new node(this);
        //        n->SetPos(realPos.x(), realPos.y());
        //        n->SetSize(150, 75);
        //        n->SetColor(QColor(0, 255, 255));
        //        Nodes.push_back(n);

        //        mState = MouseState::MouseNone;
    }
    // 空状态下，点击了一个节点
    else if (mState == MouseState::None) {
        Node *clickedNode = nullptr;
        Port *clickedPort = nullptr;
        // 判断是否有点击到
        if (targetNodeGraph->ClickDetect(realPos, clickedNode, clickedPort)) {
            // 仅仅点击到了节点，但是没有点击到接口
            if (clickedPort == nullptr) {
                targetNodeGraph->ChosenNode() = clickedNode;
                mState = MouseState::PressNode;
            } else {
                mState = MouseState::PressPort;
            }
        }
        // 点击到了空白位置
        else {
        }
    }
    update();
}

void NodeEditorTerminal::mouseMoveEvent(QMouseEvent *event) {
    // 鼠标真实的位置
    QPointF realPos = event->pos();
    realPos *= targetNodeGraph->GetCamSize();
    realPos += targetNodeGraph->GetCamPos();

    // 如果是点击了一个节点并拖动
    if (mState == MouseState::PressNode) {
        QPointF delta((realPos - targetNodeGraph->GetCamPos()) - oldMousePos);
        oldMousePos += delta;
        targetNodeGraph->ChosenNode()->Move(delta);
        //        qDebug() <<
    }
    // 如果是从一个接口引出了一条连线
    if (mState == MouseState::PressPort) {
        /// TODO
        //        node *nextNode;
        //        port *nextPort;
        //        for (int i = 0; i < Nodes.length(); i++) {
        //            Nodes[i]->IniPortState();
        //            // 寻找鼠标所在的节点
        //            if (Nodes[i]->AreaDet(realPos, nextNode, nextPort)) {
        //                // 拖到了一个新的节点上
        //                if (nextPort != nullptr) {
        //                    nextPort->state = PortState::Suspension;
        //                    break;
        //                }
        //            }
        //        }
        //        qDebug() << "正在拖出连线";
    }

    // 如果是鼠标右键点击了空白位置并拖动
    if (mState == MouseState::Grab) {
        realPos -= targetNodeGraph->GetCamPos();
        QPointF deltaPos(oldMousePos - realPos);
        targetNodeGraph->CamPosMove(deltaPos);
        oldMousePos = realPos;
    }
    update();
}

void NodeEditorTerminal::mouseReleaseEvent(QMouseEvent *event) {
    // 鼠标真实的位置
    QPointF realPos = event->pos();
    realPos *= targetNodeGraph->GetCamSize();
    realPos += targetNodeGraph->GetCamPos();

    if (mState == MouseState::PressNode) {
        mState = MouseState::None;
    }
    if (mState == MouseState::PressPort) {
        mState = MouseState::None;
        /// TODO
        //        node *nextNode;
        //        port *nextPort;
        //        for (int i = 0; i < Nodes.length(); i++) {
        //            Nodes[i]->IniPortState();
        //            // 寻找鼠标所在的节点
        //            if (Nodes[i]->AreaDet(realPos, nextNode, nextPort)) {
        //                // 拖到了一个新的节点上
        //                if (nextPort != nullptr) {
        //                    wire *w = new wire(this, chosenPort, nextPort);
        //                    Wires.push_back(w);
        //                    chosenPort->state = PortState::Linked;
        //                    nextPort->state = PortState::Linked;
        //                    break;
        //                }
        //            }
        //        }
        //        mState = MouseState::MouseNone;
        //        qDebug() << " 创建了连线";
        //        update();
    }
    // 松开鼠标右键
    if (mState == MouseState::Grab) {
        mState = MouseState::None;
    }
}

} // namespace UserInterface
