#include "userinterface_nodecreator.h"

#include "Global/globalfunc.h"
#include "Global/globalui.h"
#include "userinterface_nodesubmenu.h"

namespace UserInterface {

NodeCreator::NodeCreator(QObject *parent, NodeSubMenu *tar,
                         KernelNodeConstructorPtr cnstrctr, unsigned int num,
                         QString nam, QString desc, QString imagefilePath)
    : QObject(parent),
      targetNodeSubMenu(tar),
      title(nam),
      description(desc),
      number(num) {
    nodeConstructorEle = nodeConstructors.size();
    nodeConstructors.push_back(cnstrctr);

    state = NodeCreatorState::None;
    image = QImage(imagefilePath);
}

QVector<KernelNodeConstructorPtr> NodeCreator::nodeConstructors =
    QVector<KernelNodeConstructorPtr>();

// 位置
QRect NodeCreator::rect() {
    static QRect rc;
    rc = QRect(targetNodeSubMenu->rect().left(),
               number * globalui::node_menu_creator_height,
               targetNodeSubMenu->rect().width(),
               globalui::node_menu_creator_height);
    return rc;
}

void NodeCreator::Draw(QPainter &p) {
    // 暂存painter状态
    QBrush br = p.brush();
    QPen pe = p.pen();

    // 绘制图片
    //    QRect imageRect = rect();
    //    if (state == NodeCreatorState::Hovered) {
    //        imageRect = QRect(rect().topLeft() + QPoint(0, -5),
    //                          rect().bottomRight() + QPoint(0, 5));
    //    }
    p.drawImage(rect(), image);

    // 绘制矩形
    QLinearGradient grad(rect().topLeft(), rect().bottomLeft());
    if (state == NodeCreatorState::None) {
        grad.setColorAt(0, MultipyColor(globalui::ui_background_color, 0.8, 3));
        grad.setColorAt(1,
                        MultipyColor(globalui::ui_background_color, 0.95, 3));
    } else {
        grad.setColorAt(0, MultipyColor(globalui::ui_background_color, 0.4, 3));
        grad.setColorAt(1, MultipyColor(globalui::ui_background_color, 0.6, 3));
    }

    p.setPen(QPen(globalui::ui_foreground_color, 0));
    p.setBrush(QBrush(grad));
    p.drawRect(rect());

    // 写字
    p.setPen(MultipyColor(globalui::ui_foreground_color, 2));
    p.setFont(globalui::node_menu_title_font);
    p.drawText(QPointF(rect().topLeft().x(), rect().topLeft().y() + 40), title);

    p.setFont(globalui::node_menu_descrip_font);
    p.drawText(QPointF(rect().topLeft().x(), rect().topLeft().y() + 50), title);

    // 还原painter状态
    p.setPen(pe);
    p.setBrush(br);
}

bool NodeCreator::ClickDetect(QPointF pos) {
    if (rect().contains(QPoint(pos.x(), pos.y()))) {
        return true;
    } else
        return false;
}

} // namespace UserInterface
