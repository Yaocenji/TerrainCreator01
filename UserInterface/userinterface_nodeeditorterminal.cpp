#include "userinterface_nodeeditorterminal.h"

#include "NodeMenu/userinterface_nodecreator.h"

namespace UserInterface {

NodeEditorTerminal::NodeEditorTerminal(QWidget *parent) : QWidget(parent) {
    isPressing = false;
    // 鼠标追踪
    setMouseTracking(true);
    // 拖拽接受
    setAcceptDrops(true);
    // 键盘事件
    setFocusPolicy(Qt::ClickFocus);

    // wire的checkframebuffer初始化
    wireCheckBuffer = QPixmap(rect().width(), rect().height());
    isDebugWireCheckBuffer = false;

    mState = MouseState::None;
    oldMousePos = QPointF(0.0, 0.0);

    creatingWireSmoothInfo[0] = true;
    creatingWireSmoothInfo[1] = true;

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
    if (timerNum < 250) {
        timerNum++;
        return;
    }
    if (globalui::real_time_update) {
        update();
    }
}

void NodeEditorTerminal::SetTargetNodeGraph(NodeGraph *tar) {
    targetNodeGraph = tar;
}

void NodeEditorTerminal::RunNodeGraph(QOpenGLFunctions_4_5_Core &f) {
    targetNodeGraph->RunNodeGraph(f);
}

void NodeEditorTerminal::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p;
    p.begin(this);
    // 反走样
    p.setRenderHint(QPainter::Antialiasing, true);

    // 设置摄像头变换
    if (targetNodeGraph != nullptr) {
        p.scale(1.0 / targetNodeGraph->GetCamSize(),
                1.0 / targetNodeGraph->GetCamSize());
        p.translate(-targetNodeGraph->GetCamPos());
        p.scale(3.0, 3.0);
    }

    // 设置wire的buffer的画笔和背景
    wireCheckBuffer.fill(Qt::white);
    QPainter pwfb(&wireCheckBuffer);
    //    pwfb.setRenderHint(QPainter::Antialiasing, true);
    if (targetNodeGraph != nullptr) {
        pwfb.scale(1.0 / targetNodeGraph->GetCamSize(),
                   1.0 / targetNodeGraph->GetCamSize());
        pwfb.translate(-targetNodeGraph->GetCamPos());
    }

    // 设置画笔，绘制背景
    //
    QBrush br = QBrush(globalui::editor_background_color);
    br.setStyle(Qt::BrushStyle::SolidPattern);

    p.setPen(QColor(0, 0, 0, 0));
    p.setBrush(br);
    p.drawRect(targetNodeGraph->GetCamPos().x() / 3.0 - 7.0,
               targetNodeGraph->GetCamPos().y() / 3.0 - 7.0,
               this->width() * targetNodeGraph->GetCamSize() / 3.0 + 14.0,
               this->height() * targetNodeGraph->GetCamSize() / 3.0 + 14.0);

    //    br.setColor(globalui::editor_foreground_color);
    //    br.setStyle(Qt::BrushStyle::Dense7Pattern);
    //    p.setBrush(br);
    //    p.drawRect(targetNodeGraph->GetCamPos().x() / 3.0 - 7.0,
    //               targetNodeGraph->GetCamPos().y() / 3.0 - 7.0,
    //               this->width() * targetNodeGraph->GetCamSize() / 3.0 + 14.0,
    //               this->height() * targetNodeGraph->GetCamSize() / 3.0
    //               + 14.0);

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
                           realMousePos, creatingWireSmoothInfo[0],
                           creatingWireSmoothInfo[1]);
        targetNodeGraph->Draw(p);
        targetNodeGraph->DrawWireCheckFrameBuffer(pwfb);
    }
    p.end();

    if (isDebugWireCheckBuffer) {
        QPainter debugPainter(this);
        debugPainter.drawPixmap(rect(), wireCheckBuffer);
    }
}

void NodeEditorTerminal::mousePressEvent(QMouseEvent *event) {
    isPressing = true;
    // 真实的位置
    realMousePos = event->pos();
    realMousePos *= targetNodeGraph->GetCamSize();
    realMousePos += targetNodeGraph->GetCamPos();

    oldMousePos = realMousePos - targetNodeGraph->GetCamPos();
    // 如果是右键点击，那么：
    if (event->button() == Qt::MidButton) {
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
    // 空状态下，点击了一个元素
    else if (mState == MouseState::None) {
        QPointF ssPos = event->pos();
        Node *clickedNode = nullptr;
        Port *clickedPort = nullptr;
        Wire *clickedWire = nullptr;
        // 判断是否有点击到
        if (targetNodeGraph->ClickDetectNodePort(realMousePos, clickedNode,
                                                 clickedPort)) {
            // 仅仅点击到了节点，但是没有点击到接口
            if (clickedPort == nullptr) {
                targetNodeGraph->ChooseOneNode(clickedNode);
                mState = MouseState::PressNode;
            } else {
                targetNodeGraph->ChooseOneNode(clickedNode);
                targetNodeGraph->ChosenPort() = clickedPort;
                mState = MouseState::PressPort;

                // 根据悬浮的接口类型改变曲线绘制类型
                if (clickedPort->GetTargetKernelPort()->GetType() ==
                    Kernel::PortType::Param)
                    creatingWireSmoothInfo[0] = false;
                else
                    creatingWireSmoothInfo[0] = true;
            }
        }
        // 否则判断是否点击到了wire
        else if (targetNodeGraph->ClickDetectWire(ssPos, clickedWire,
                                                  wireCheckBuffer)) {
            // 左键选中，右键删除
            if (event->button() == Qt::LeftButton) {
                targetNodeGraph->ChooseOneWire(clickedWire);
                mState = MouseState::PressWire;
            } else if (event->button() == Qt::RightButton) {
                targetNodeGraph->DeleteWire(clickedWire);
            }
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
    // 窗口空间的鼠标坐标
    QPointF ssPos = event->pos();

    // 一些辅助用的静态变量
    // 这部分还没有，先预留位置

    if (!isPressing) {
        Node *clickedNode = nullptr;
        Port *clickedPort = nullptr;
        Wire *clickedWire = nullptr;
        // 判断是否有点击到
        targetNodeGraph->ClickDetectNodePort(realMousePos, clickedNode,
                                             clickedPort);
        targetNodeGraph->ClickDetectWire(ssPos, clickedWire, wireCheckBuffer);
        // 仅仅点击到了节点，但是没有点击到接口
        if (clickedPort != nullptr) {
            targetNodeGraph->PortHoveredUpdate(clickedPort);
        } else {
            targetNodeGraph->PortHoveredUpdate(nullptr);
        }
        // 鼠标悬停在wire上
        if (clickedWire != nullptr) {
            targetNodeGraph->WireHoveredUpdate(clickedWire);
        } else {
            targetNodeGraph->WireHoveredUpdate(nullptr);
        }
    } else {
        // 如果是点击了一个节点并拖动
        if (mState == MouseState::PressNode) {
            QPointF delta((realMousePos - targetNodeGraph->GetCamPos()) -
                          oldMousePos);
            oldMousePos += delta;
            targetNodeGraph->MoveChosenNodes(delta);
        }
        // 如果是从一个接口引出了一条连线
        if (mState == MouseState::PressPort) {
            /// TODO
            Node *clickedNode = nullptr;
            Port *clickedPort = nullptr;
            // 判断是否有点击到
            if (targetNodeGraph->ClickDetectNodePort(realMousePos, clickedNode,
                                                     clickedPort)) {
                // 悬浮在接口上
                if (clickedPort != nullptr) {
                    targetNodeGraph->PortHoveredUpdate(clickedPort);

                    // 根据悬浮的接口类型改变曲线绘制类型
                    if (clickedPort->GetTargetKernelPort()->GetType() ==
                        Kernel::PortType::Param)
                        creatingWireSmoothInfo[1] = false;
                    else
                        creatingWireSmoothInfo[1] = true;
                } else {
                    // 根据悬浮的接口类型改变曲线绘制类型
                    creatingWireSmoothInfo[1] = true;
                }
            } else {
                // 根据悬浮的接口类型改变曲线绘制类型
                creatingWireSmoothInfo[1] = true;
            }
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
        if (targetNodeGraph->ClickDetectNodePort(realMousePos, clickedNode,
                                                 clickedPort)) {
            // 点击到了节点
            if (clickedPort != nullptr) {
                targetNodeGraph->LinkWire(targetNodeGraph->ChosenPort(),
                                          clickedPort);
                targetNodeGraph->ChosenPort() = clickedPort;
            }
        }
        mState = MouseState::None;
        update();
    }
    if (mState == MouseState::PressWire) {
        mState = MouseState::None;
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

void NodeEditorTerminal::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text")) {
        event->accept();
        qDebug() << "[recvWidget]: dragEnterEvent accept";
    } else
        event->ignore();

    update();
}

void NodeEditorTerminal::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}

void NodeEditorTerminal::dropEvent(QDropEvent *event) {
    qDebug() << "[recvWidget]:dropEvent drop";

    // 鼠标真实的位置
    QPointF realMousePos = event->pos();
    realMousePos *= targetNodeGraph->GetCamSize();
    realMousePos += targetNodeGraph->GetCamPos();

    if (event->mimeData()->hasFormat("text")) {
        // 接受数据，并且转换为索引
        QByteArray byteArray = event->mimeData()->data("text");
        //        m_label->setText("recv:" + QString(byteArray));
        int ptrEle = 0;
        bool isOk = false;
        ptrEle = QString(byteArray).toInt(&isOk);
        if (!isOk)
            qDebug()
                << "错误：拖动创建节点时，从QMime数据中解包的节点构造函数指针"
                   "数组索引数据不是int";
        // 获取节点构造函数指针
        KernelNodeConstructorPtr targetptr =
            NodeCreator::nodeConstructors[ptrEle];
        // 构造节点
        targetNodeGraph->addNodeByConstructor(realMousePos, targetptr);
        event->setDropAction(Qt::MoveAction);
        // 结束
        event->accept();
    } else
        event->ignore();

    update();
}

void NodeEditorTerminal::resizeEvent(QResizeEvent *event) {
    // wire的checkframebuffer重新初始化
    wireCheckBuffer = QPixmap(rect().width(), rect().height());
}

void NodeEditorTerminal::keyPressEvent(QKeyEvent *event) {
    qDebug() << "键盘事件";
    if (event->key() == Qt::Key_Delete) {
        // 删除节点
        qDebug() << "删除节点";
        targetNodeGraph->DeleteChosenNode();
    }
}

} // namespace UserInterface
