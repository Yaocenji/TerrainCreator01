#include "userinterface_nodesubmenu.h"

#include "./Global/globalui.h"
#include "userinterface_nodecreator.h"
#include "userinterface_nodemenu.h"

namespace UserInterface {

NodeSubMenu::NodeSubMenu(QWidget *parent, NodeMenu *target)
    : QObject(parent), targetNodeMenu(target) {
}

QRect NodeSubMenu::rect() {
    return targetNodeMenu->subMenuRect();
}

void NodeSubMenu::addNodeCreator(KernelNodeConstructorPtr cnstr, QString tit,
                                 QString desc, QString imPath) {
    nodeCreators.push_back(new NodeCreator(
        this, this, cnstr, nodeCreators.size(), tit, desc, imPath));
}

void NodeSubMenu::Draw(QPainter &p, int verticalCamOffset) {
    QBrush br = p.brush();
    QPen pe = p.pen();

    p.setBrush(QBrush(globalui::ui_background_color));
    p.setPen(QPen(globalui::transparent_color));

    p.drawRect(rect());

    // 摄像机变换
    p.translate(QPointF(0, verticalCamOffset));

    for (auto &i : nodeCreators) {
        i->Draw(p);
    }

    p.translate(QPointF(0, -verticalCamOffset));

    // 临时代码
    // 写字
    //    p.setFont(globalui::node_menu_title_font);
    //    p.drawText(this->rect(), QString::number(number));

    //    p.setFont(globalui::node_menu_descrip_font);
    //    p.drawText(QPointF(rect().topLeft().x(), rect().topLeft().y() + 50),
    //    title);
}

} // namespace UserInterface
