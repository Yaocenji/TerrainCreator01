#ifndef USERINTERFACE_OPTION_H
#define USERINTERFACE_OPTION_H

#include <QColor>
#include <QPointF>
#include <QVector2D>

namespace UserInterface {

/// 全局UI显示信息
class Option {
public:
    /// 节点基础绘制信息
    /// 输入节点的前后边距
    static float input_column_found_size[2];
    /// 输出节点的前后边距
    static float output_column_found_size[2];
    /// 参数节点的前后边距
    static float param_line_found_size[2];
    /// 节点显示缩放
    static float node_scale;
    /// 圆角半径
    static float node_angle_radius;
    /// 节点轮廓绘制粗细
    static float pen_width;

    /// 接口之间的间隔
    static float port_interval_size;
    /// 接口的半径
    static float port_radius;
    /// 接口的大小缩放
    static float port_scale;

    /// 背景色
    static QColor background_color;
    /// 前景色
    static QColor foreground_color;

public:
    Option();
};

} // namespace UserInterface

#endif // USERINTERFACE_OPTION_H
