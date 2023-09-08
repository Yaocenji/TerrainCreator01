#include "userinterface_nodemenu.h"

#include "./Global/globalfunc.h"
#include "./Global/globalui.h"
#include "Kernel/kernel_element_inc.h"
#include "userinterface_nodecreator.h"
#include "userinterface_nodemenubutton.h"
#include "userinterface_nodesubmenu.h"

namespace UserInterface {

NodeMenu::NodeMenu(QWidget *parent) : QWidget(parent) {
    InitMenu();
    isPressing = false;
    setMouseTracking(true);
    pressModel = 0;
    buttonBarCamPos = 0;
    subMenuCamPos = 0;
}

void NodeMenu::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    // 反走样
    p.setRenderHint(QPainter::Antialiasing, true);

    p.setBrush(QBrush(MultipyColor(globalui::ui_background_color, 0.75)));
    p.setPen(QPen(globalui::transparent_color));

    p.drawRect(rect());

    chosenSubMenu->Draw(p, subMenuCamPos);

    for (auto &i : menuButtons) {
        i->Draw(p);
    }
}

QPointF NodeMenu::camOffset(QPointF p) {
    QPointF ans = p;
    if (p.x() <= globalui::node_menu_button_width) {
        ans -= QPointF(0, buttonBarCamPos);
    } else {
        ans -= QPointF(0, subMenuCamPos);
    }
    return ans;
}
QPoint NodeMenu::camOffset(QPoint p) {
    QPoint ans = p;
    if (p.x() <= globalui::node_menu_button_width) {
        ans -= QPoint(0, buttonBarCamPos);
    } else {
        ans -= QPoint(0, subMenuCamPos);
    }
    return ans;
}

void NodeMenu::InitMenu() {
    addSubMenu();
    addSubMenu();
    chosenSubMenu = subMenus[0];

    subMenus[0]->addNodeCreator(
        [](QObject *parent, Kernel::NodeGraph *nodeGraph) {
            return (Kernel::Node *)(new Kernel::PerlinNoise_Node(parent,
                                                                 nodeGraph));
        },
        "柏林噪声", "生成二维柏林噪声高度场", ":/test/test/91514322_p1.png");

    subMenus[1]->addNodeCreator(
        [](QObject *parent, Kernel::NodeGraph *nodeGraph) {
            return (Kernel::Node *)(new Kernel::Clamp_Node(parent, nodeGraph));
        },
        "钳制", "设置高度场上下限", ":/test/test/91514322_p1.png");
}

void NodeMenu::addSubMenu() {
    subMenus.push_back(new NodeSubMenu(this, this));
    menuButtons.push_back(
        new NodeMenuButton(this, this, menuButtons.size(), "", ""));
}

bool NodeMenu::chooseMenuButton(int ch) {
    qDebug() << "选择了菜单" << ch;
    if (ch >= menuButtons.size() || ch < 0) {
        qDebug()
            << "错误：调用chooseMenu函数参数错误（参数小于0或大于数组长度）";
        return false;
    }
    bool flag = false;
    for (int i = 0; i < menuButtons.size(); i++) {
        if (i == ch) {
            chosenMenuButton = menuButtons[i];
            menuButtons[i]->state = NodeMenuButtonState::Pressed;

            chooseSubMenu(ch);
            flag = true;
        } else {
            menuButtons[i]->state = NodeMenuButtonState::None;
        }
    }
    return flag;
}

bool NodeMenu::chooseMenuButton(NodeMenuButton *ch) {
    bool flag = false;
    for (int i = 0; i < menuButtons.size(); i++) {
        if (menuButtons[i] == ch) {
            chosenMenuButton = menuButtons[i];
            menuButtons[i]->state = NodeMenuButtonState::Pressed;

            chooseSubMenu(i);
            flag = true;
        } else {
            menuButtons[i]->state = NodeMenuButtonState::None;
        }
    }
    return flag;
}

bool NodeMenu::chooseSubMenu(int ch) {
    if (ch >= subMenus.size() || ch < 0) {
        qDebug()
            << "错误：调用chooseMenu函数参数错误（参数小于0或大于数组长度）";
        return false;
    }
    for (int i = 0; i < subMenus.size(); i++) {
        if (i == ch) {
            chosenSubMenu = subMenus[i];
            return true;
        }
    }
    return false;
}

bool NodeMenu::chooseSubMenu(NodeSubMenu *ch) {
    for (auto &i : subMenus) {
        if (i == ch) {
            chosenSubMenu = i;
            return true;
        }
    }
    return false;
}

QRect NodeMenu::subMenuRect() {
    static QRect smRect;
    smRect = QRect(QPoint(globalui::node_menu_button_width, 0),
                   rect().bottomRight());
    return smRect;
}

void NodeMenu::mousePressEvent(QMouseEvent *event) {
    isPressing = true;
    if (mousePressPos.x() <= globalui::node_menu_button_width)
        pressModel = 0;
    else
        pressModel = 1;

    mousePressPos = camOffset(event->pos());

    mouseOldPos = mousePressPos;

    update();
}

void NodeMenu::mouseMoveEvent(QMouseEvent *event) {
    if (!isPressing) {
        static QPointF mousePos;
        mousePos = camOffset(event->pos());
        // 如果鼠标在左侧范围
        if (mousePos.x() <= globalui::node_menu_button_width) {
            NodeMenuButton *clicked = nullptr;
            // 依次进行鼠标检测
            for (auto &i : menuButtons) {
                if (i->ClickDetect(mousePos)) {
                    if (i->state != NodeMenuButtonState::Pressed)
                        i->state = NodeMenuButtonState::Hovered;
                    clicked = i;
                    break;
                }
            }
            // 对所有没有点击到的做处理
            for (auto &i : menuButtons) {
                if (i != clicked && i->state != NodeMenuButtonState::Pressed) {
                    i->state = NodeMenuButtonState::None;
                }
            }
            for (auto &i : chosenSubMenu->nodeCreators) {
                i->state = NodeCreatorState::None;
            }
        }
        // 如果鼠标在右侧范围
        else {
            NodeCreator *clicked = nullptr;
            // 依次进行鼠标检测
            for (auto &i : chosenSubMenu->nodeCreators) {
                if (i->ClickDetect(mousePos)) {
                    i->state = NodeCreatorState::Hovered;
                    clicked = i;
                    break;
                }
            }
            // 对所有没有点击到的做处理
            for (auto &i : chosenSubMenu->nodeCreators) {
                if (i != clicked) {
                    i->state = NodeCreatorState::None;
                }
            }
            for (auto &i : menuButtons) {
                if (i->state != NodeMenuButtonState::Pressed)
                    i->state = NodeMenuButtonState::None;
            }
        }
        mouseOldPos = mousePos;
    } else {
        if (pressModel == 1) {
            static QPointF mousePos;
            mousePos = camOffset(event->pos());
            // TODO 拖拽事件
            // 判断拖动长度是否超过阈值
            if ((mousePressPos - mousePos).manhattanLength() >= 5) {
                // 判断鼠标是否点击
                static NodeCreator *chosenNodeCreator;
                chosenNodeCreator = nullptr;
                for (auto &i : chosenSubMenu->nodeCreators) {
                    if (i->rect().contains(
                            QPoint(mousePos.x(), mousePos.y()))) {
                        chosenNodeCreator = i;
                        break;
                    }
                }
                // 开始拖动
                if (chosenNodeCreator != nullptr) {
                    QDrag *drag = new QDrag(this);

                    QMimeData *mimeData = new QMimeData();

                    QPixmap pixmap(":/test/test/91514322_p1.png");
                    drag->setPixmap(pixmap);
                    drag->setHotSpot(QPoint(20, 30));

                    drag->setMimeData(mimeData);

                    QByteArray byteArray(
                        QString::number(chosenNodeCreator->nodeConstructorEle)
                            .toUtf8());

                    mimeData->setData("text", byteArray);

                    drag->exec();
                }
            }
        }
        mouseOldPos = camOffset(event->pos());
    }

    // 如果鼠标碰到边界，那么所有的hover状态变为none
    QRect innerRect = QRect(QPoint(5, 5), rect().bottomRight() - QPoint(5, 5));
    if (!innerRect.contains(event->pos())) {
        for (auto &i : menuButtons) {
            if (i->state != NodeMenuButtonState::Pressed)
                i->state = NodeMenuButtonState::None;
        }
        for (auto &i : chosenSubMenu->nodeCreators) {
            i->state = NodeCreatorState::None;
        }
    }

    update();
}

void NodeMenu::mouseReleaseEvent(QMouseEvent *event) {
    isPressing = false;
    mouseReleasePos = camOffset(event->pos());

    // 点击了左侧菜单
    if (pressModel == 0) {
        for (int i = 0; i < menuButtons.size(); i++) {
            // 如果点击与松开的坐标都检测到同一个按钮上
            if (menuButtons[i]->ClickDetect(mousePressPos) &&
                menuButtons[i]->ClickDetect(mouseReleasePos)) {
                // 该按钮被点击时
                chooseMenuButton(i);
                break;
            }
        }
    }

    update();
}

void NodeMenu::wheelEvent(QWheelEvent *event) {
    QPointF pos = camOffset(event->pos());
    int delta = event->delta();
    if (delta > 0) {
        if (pos.x() <= globalui::node_menu_button_width)
            buttonBarCamPos += 15;
        else
            subMenuCamPos += 15;
    } else if (delta < 0) {
        if (pos.x() <= globalui::node_menu_button_width)
            buttonBarCamPos -= 15;
        else
            subMenuCamPos -= 15;
    }
    // 框定范围
    if (pos.x() <= globalui::node_menu_button_width) {
        int max_hi = menuButtons.size() * globalui::node_menu_button_height;
        if (max_hi <= rect().height())
            buttonBarCamPos = 0;
        else {
            buttonBarCamPos =
                clamp<int>(0, rect().height() - max_hi, buttonBarCamPos);
        }
    } else {
        int max_hi = chosenSubMenu->nodeCreators.size() *
                     globalui::node_menu_creator_height;
        if (max_hi <= rect().height())
            subMenuCamPos = 0;
        else {
            subMenuCamPos =
                clamp<int>(0, rect().height() - max_hi, subMenuCamPos);
        }
    }
    QWidget::wheelEvent(event); // 将事件传递给父类处理
    // 手动调用一次move事件
    QMouseEvent eve(QEvent::MouseMove, event->pos(), Qt::NoButton, Qt::NoButton,
                    event->modifiers());
    QApplication::sendEvent(this, &eve);
    // 更新绘制
    update();
}

} // namespace UserInterface
