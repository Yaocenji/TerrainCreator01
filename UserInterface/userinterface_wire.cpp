#include "userinterface_wire.h"

#include "../Global/globalui.h"
#include "Global/globalfunc.h"
#include "userinterface_port.h"

namespace UserInterface {

Wire::Wire(QObject *parent, Kernel::Wire *tar, Port *lp1, Port *lp2)
    : QObject(parent), targetWire(tar) {
    tar->targetUIWire = this;
    // 整理：linkedPort 0为输出，1为输入
    if (lp1->GetTargetKernelPort()->GetType() == Kernel::PortType::Output) {
        linkedPort[0] = lp1;
        linkedPort[1] = lp2;
    } else {
        linkedPort[0] = lp2;
        linkedPort[1] = lp1;
    }
    bool checkColorCreateAns = createCheckColor();
    if (!checkColorCreateAns) {
        qDebug() << "连线的checkColor创建失败！";
    }
}

QVector<QColor> Wire::checkColors = QVector<QColor>();

void Wire::Draw(QPainter &p) {
    // 设置绘制样式
    bool sm_1 = true, sm_2 = true;
    // 设置过渡方向
    if (linkedPort[0]->GetTargetKernelPort()->GetType() ==
        Kernel::PortType::Param) {
        sm_1 = false;
    }
    if (linkedPort[1]->GetTargetKernelPort()->GetType() ==
        Kernel::PortType::Param) {
        sm_2 = false;
    }

    if (state == WireChosenState::Chosen)
        DrawWire(p, globalui::node_chosen_color, 11,
                 linkedPort[0]->GetWorldPos(), linkedPort[1]->GetWorldPos(),
                 sm_1, sm_2);
    else if (state == WireChosenState::Hovered) {
        DrawWire(p, MultipyColor(globalui::node_chosen_color, 0.5, 3), 11,
                 linkedPort[0]->GetWorldPos(), linkedPort[1]->GetWorldPos(),
                 sm_1, sm_2);
    }
    DrawWire(p, linkedPort[0]->GetWorldPos(), linkedPort[1]->GetWorldPos(),
             sm_1, sm_2);
}

void Wire::DrawFrameBuffer(QPainter &p) {
    // 设置绘制样式
    bool sm_1 = true, sm_2 = true;
    // 设置过渡方向
    if (linkedPort[0]->GetTargetKernelPort()->GetType() ==
        Kernel::PortType::Param) {
        sm_1 = false;
    }
    if (linkedPort[1]->GetTargetKernelPort()->GetType() ==
        Kernel::PortType::Param) {
        sm_2 = false;
    }

    DrawWire(p, checkColors[checkColorIndex], linkedPort[0]->GetWorldPos(),
             linkedPort[1]->GetWorldPos(), sm_1, sm_2);
}

bool Wire::ClickDetect(QPointF pos, Wire *&clickedWire, QPixmap &fb) {
    if (fb.toImage().pixelColor(QPoint(pos.x(), pos.y())) ==
        checkColors[checkColorIndex]) {
        clickedWire = this;
        return true;
    } else
        return false;
}

void Wire::DrawWire(QPainter &p, QPointF pos1, QPointF pos2, bool pos1_smooth,
                    bool pos2_smooth) {
    QBrush br = p.brush();
    p.setBrush(QBrush(globalui::transparent_color));

    p.setPen(QPen(globalui::wire_color, 6));

    DrawPathKernel(p, pos1, pos2, pos1_smooth, pos2_smooth);

    p.setBrush(br);
}

void Wire::DrawWire(QPainter &p, QColor col, QPointF pos1, QPointF pos2,
                    bool pos1_smooth, bool pos2_smooth) {
    QBrush br = p.brush();
    p.setBrush(QBrush(globalui::transparent_color));

    p.setPen(QPen(col, 6));

    DrawPathKernel(p, pos1, pos2, pos1_smooth, pos2_smooth);

    p.setBrush(br);
}

void Wire::DrawWire(QPainter &p, QColor col, int pen_width, QPointF pos1,
                    QPointF pos2, bool pos1_smooth, bool pos2_smooth) {
    QBrush br = p.brush();
    p.setBrush(QBrush(globalui::transparent_color));

    p.setPen(QPen(col, pen_width));
    DrawPathKernel(p, pos1, pos2, pos1_smooth, pos2_smooth);

    p.setBrush(br);
}

void Wire::DrawPathKernel(QPainter &p, QPointF pos1, QPointF pos2,
                          bool pos1_smooth, bool pos2_smooth) {
    QPointF beginPos = pos1;
    QPointF endPos = pos2;

    float dist =
        sqrtf(powf(pos1.x() - pos2.x(), 2.0) + powf(pos1.y() - pos2.y(), 2.0));
    QPointF c1 = QPointF(pos1.x() + (3.0f + dist * 0.1f), pos1.y());
    QPointF c2 = QPointF(pos2.x() - (3.0f + dist * 0.1f), pos2.y());

    // 修正画法：
    if (!pos1_smooth && pos2_smooth) {
        c2 = QPointF(c2.x() - (3.0f + dist * 0.12f), c2.y());

        QPainterPath path;
        path.moveTo(endPos);
        path.quadTo(c2, beginPos);
        p.drawPath(path);
    } else if (!pos2_smooth && pos1_smooth) {
        c1 = QPointF(c1.x() + (3.0f + dist * 0.12f), c1.y());

        QPainterPath path;
        path.moveTo(beginPos);
        path.quadTo(c1, endPos);
        p.drawPath(path);
    } else if (!pos1_smooth && !pos2_smooth) {
        p.drawLine(beginPos, endPos);
    } else {
        QPainterPath path;
        path.moveTo(beginPos);
        path.quadTo(c1, (beginPos + endPos) / 2.0f);
        path.quadTo(c2, endPos);

        p.drawPath(path);
    }
}

bool Wire::createCheckColor() {
    // 随机生成该wire的判断颜色
    int red_c = 0;
    int green_c = 0;
    int blue_c = 0;
    bool flag = false;
    for (flag = false; red_c < 255; red_c++) {
        // 检测该分量是否有还没用过的值
        bool f = true;
        for (auto &i : checkColors) {
            if (i.red() == red_c) {
                f = false;
                break;
            }
        }
        if (f) {
            flag = true;
            break;
        }
    }
    if (flag) {
        checkColorIndex = checkColors.size();
        checkColors.push_back(QColor(red_c, green_c, blue_c));
        return true;
    }
    for (flag = false, red_c = 0; green_c < 255; green_c++) {
        // 检测该分量是否有还没用过的值
        bool f = true;
        for (auto &i : checkColors) {
            if (i.green() == green_c) {
                f = false;
                break;
            }
        }
        if (f) {
            flag = true;
            break;
        }
    }
    if (flag) {
        checkColorIndex = checkColors.size();
        checkColors.push_back(QColor(red_c, green_c, blue_c));
        return true;
    }
    for (flag = false, red_c = 0, green_c = 0; blue_c < 255; blue_c++) {
        // 检测该分量是否有还没用过的值
        bool f = true;
        for (auto &i : checkColors) {
            if (i.blue() == blue_c) {
                f = false;
                break;
            }
        }
        if (f) {
            flag = true;
            break;
        }
    }
    if (flag) {
        checkColorIndex = checkColors.size();
        checkColors.push_back(QColor(red_c, green_c, blue_c));
        return true;
    }
    // 无法创建出颜色：几乎不会运行到该分支中
    return false;
}

bool Wire::deleteCheckColor() {
    int i = 0;
    for (QVector<QColor>::iterator it = checkColors.begin();
         it != checkColors.end(); it++, i++) {
        if (i == checkColorIndex) {
            it = checkColors.erase(it);
            return true;
        }
    }
    return false;
}

} // namespace UserInterface
