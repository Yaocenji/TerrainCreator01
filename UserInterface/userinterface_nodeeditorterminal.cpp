#include "userinterface_nodeeditorterminal.h"

namespace UserInterface {

NodeEditorTerminal::NodeEditorTerminal(QWidget *parent) : QWidget(parent) {
    isPressing = false;
    setMouseTracking(true);
    mState = MouseState::None;
    oldMousePos = QPointF(0.0, 0.0);
    // 启动计时器
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(33);
    timer->start();
    // 根据计时器信号更新画面
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
}

void NodeEditorTerminal::TimerUpdate() {
    static unsigned int timerNum = 0;
    if (timerNum < 250)
        timerNum++;
    else if (Option::real_time_update)
        update();
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
        // 如果正在创建一个连线，绘制预览
        if (mState == MouseState::PressPort)
            Wire::DrawWire(p, targetNodeGraph->ChosenPort()->GetWorldPos(),
                           realMousePos);
        targetNodeGraph->Draw(p);
    }
}

void NodeEditorTerminal::mousePressEvent(QMouseEvent *event) {
    //    qDebug() << "PRESS";
    isPressing = true;
    // 真实的位置
    realMousePos = event->pos();
    realMousePos *= targetNodeGraph->GetCamSize();
    realMousePos += targetNodeGraph->GetCamPos();

    oldMousePos = realMousePos - targetNodeGraph->GetCamPos();
    // 如果是右键点击，那么：
    if (event->button() == Qt::RightButton) {
        // 如果目前是创建模式，那么取消创建
        if (mState == MouseState::CreateNode) {
            // TODO 取消创建模式
        }
        // 如果目前是空状态，那么进入抓住拖动模式
        if (mState == MouseState::None) {
            mState = MouseState::Grab;
            //            realMousePos -= targetNodeGraph->GetCamPos();
            //            oldMousePos = realMousePos;
        }

        update();
        return;
    }
    // 如果是鼠标左键
    // 创建一个新的节点
    if (mState == MouseState::CreateNode) {
        /// TODO
        //        node *n = new node(this);
        //        n->SetPos(realMousePos.x(), realMousePos.y());
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
        if (targetNodeGraph->ClickDetect(realMousePos, clickedNode,
                                         clickedPort)) {
            // 仅仅点击到了节点，但是没有点击到接口
            if (clickedPort == nullptr) {
                targetNodeGraph->ChosenNode() = clickedNode;
                mState = MouseState::PressNode;
            } else {
                targetNodeGraph->ChosenNode() = clickedNode;
                targetNodeGraph->ChosenPort() = clickedPort;
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
    update();
    // 鼠标真实的位置
    realMousePos = event->pos();
    realMousePos *= targetNodeGraph->GetCamSize();
    realMousePos += targetNodeGraph->GetCamPos();

    // 一些辅助用的静态变量
    // 这部分还没有，先预留位置

    if (!isPressing) {
        Node *clickedNode = nullptr;
        Port *clickedPort = nullptr;
        // 判断是否有点击到
        targetNodeGraph->ClickDetect(realMousePos, clickedNode, clickedPort);
        // 仅仅点击到了节点，但是没有点击到接口
        if (clickedPort != nullptr) {
            targetNodeGraph->PortSuspensionUpdate(clickedPort);
        } else {
            targetNodeGraph->PortSuspensionUpdate(nullptr);
        }
    } else {
        // 如果是点击了一个节点并拖动
        if (mState == MouseState::PressNode) {
            QPointF delta((realMousePos - targetNodeGraph->GetCamPos()) -
                          oldMousePos);
            oldMousePos += delta;
            targetNodeGraph->ChosenNode()->Move(delta);
            //        qDebug() <<
        }
        // 如果是从一个接口引出了一条连线
        if (mState == MouseState::PressPort) {
            /// TODO
            Node *clickedNode = nullptr;
            Port *clickedPort = nullptr;
            // 判断是否有点击到
            if (targetNodeGraph->ClickDetect(realMousePos, clickedNode,
                                             clickedPort)) {
                // 仅仅点击到了节点，但是没有点击到接口
                if (clickedPort != nullptr) {
                    targetNodeGraph->PortSuspensionUpdate(clickedPort);
                }
            }
            qDebug() << "正在拖出连线";
        }

        // 如果是鼠标右键点击了空白位置并拖动
        if (mState == MouseState::Grab) {
            realMousePos -= targetNodeGraph->GetCamPos();
            QPointF deltaPos(oldMousePos - realMousePos);
            targetNodeGraph->CamPosMove(deltaPos);
            oldMousePos = realMousePos;
        }
    }
    update();
}

void NodeEditorTerminal::mouseReleaseEvent(QMouseEvent *event) {
    //    qDebug() << "RELEASE";
    isPressing = false;
    // 鼠标真实的位置
    QPointF realMousePos = event->pos();
    realMousePos *= targetNodeGraph->GetCamSize();
    realMousePos += targetNodeGraph->GetCamPos();

    if (mState == MouseState::PressNode) {
        mState = MouseState::None;
    }
    if (mState == MouseState::PressPort) {
        mState = MouseState::None;
        /// TODO
        Node *clickedNode = nullptr;
        Port *clickedPort = nullptr;
        // 判断是否有点击到
        if (targetNodeGraph->ClickDetect(realMousePos, clickedNode,
                                         clickedPort)) {
            // 点击到了节点
            if (clickedPort != nullptr) {
                targetNodeGraph->LinkWire(targetNodeGraph->ChosenPort(),
                                          clickedPort);
                targetNodeGraph->ChosenPort() = clickedPort;
            }
            qDebug() << " 创建了连线";
        }
        mState = MouseState::None;
        update();
    }
    // 松开鼠标右键
    if (mState == MouseState::Grab) {
        mState = MouseState::None;
    }
}

void NodeEditorTerminal::wheelEvent(QWheelEvent *event) {
    QPointF Center =
        targetNodeGraph->GetCamPos() +
        QPointF(width(), height()) * targetNodeGraph->GetCamSize() * 0.5f;
    if (event->delta() < 0) { // 当滚轮远离使用者时
        targetNodeGraph->GetCamSizeCtrl() += 0.05f;
        targetNodeGraph->GetCamSize() = exp(targetNodeGraph->GetCamSizeCtrl());

    } else { // 当滚轮向使用者方向旋转时
             // 最大缩放
             // if (CameraSizeMagn > 0.401f) {
        // 保证画面中心不改变
        targetNodeGraph->GetCamSizeCtrl() -= 0.05f;
        targetNodeGraph->GetCamSize() = exp(targetNodeGraph->GetCamSizeCtrl());
        //}
    }
    targetNodeGraph->GetCamPos() = Center - QPointF(width(), height()) *
                                                targetNodeGraph->GetCamSize() *
                                                0.5f;
    // if (CameraSizeMagn <= 0.4f) CameraSizeMagn = 0.4f;
    update();
}

} // namespace UserInterface
