#include "userinterface_nodemenubutton.h"

#include "Global/globalfunc.h"
#include "Global/globalui.h"
#include "userinterface_nodemenu.h"

namespace UserInterface {

NodeMenuButton::NodeMenuButton(QObject *parent, NodeMenu *tar, unsigned int num,
                               QString tit, QString imagefilePath)
    : QObject(parent), targetNodeMenu(tar), title(tit), number(num) {
    state = NodeMenuButtonState::None;
}

// 位置
QRect NodeMenuButton::rect() {
    static QRect rc;
    rc = QRect(0, number * globalui::node_menu_button_height,
               globalui::node_menu_button_width,
               globalui::node_menu_button_height);
    return rc;
}

void NodeMenuButton::Draw(QPainter &p) {
    // 暂存painter状态
    QBrush br = p.brush();
    QPen pe = p.pen();

    // TODO 绘制图片

    // 绘制矩形
    p.setPen(QPen(globalui::transparent_color));

    QColor brCol;
    if (state == NodeMenuButtonState::Pressed)
        brCol = AverageColor(globalui::ui_foreground_color,
                             globalui::ui_background_color);
    else if (state == NodeMenuButtonState::Hovered)
        brCol =
            AverageColor(globalui::ui_background_color,
                         brCol = AverageColor(globalui::ui_foreground_color,
                                              globalui::ui_background_color));
    else
        brCol = globalui::ui_background_color;

    p.setBrush(QBrush(brCol));
    p.drawRect(rect());

    // 写字
    //    p.setFont(globalui::node_menu_title_font);
    //    p.drawText(QPointF(rect().topLeft().x(), rect().topLeft().y() + 40),
    //    title);

    //    p.setFont(globalui::node_menu_descrip_font);
    //    p.drawText(QPointF(rect().topLeft().x(), rect().topLeft().y() + 50),
    //    title);

    // 还原painter状态
    p.setPen(pe);
    p.setBrush(br);
}

bool NodeMenuButton::ClickDetect(QPointF pos) {
    if (rect().contains(QPoint(pos.x(), pos.y()))) {
        return true;
    } else
        return false;
}

} // namespace UserInterface
