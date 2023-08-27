#ifndef GLOBALUI
#define GLOBALUI

#include <QColor>
#include <QFont>
#include <QPen>
#include <QPointF>
#include <QVector2D>

namespace UserInterface {

/// 全局UI显示信息
class globalui {
public:
    /// 是否要随时绘制（性能开销高）
    static bool real_time_update;

    /// 轮廓绘制粗细
    static float pen_width;

    /// 透明色
    static QColor transparent_color;

    /// UI字体
    static QFont ui_font;
    /// UI圆角半径
    static float ui_angle_radius;
    /// UI背景颜色
    static QColor ui_background_color;
    /// UI前景颜色
    static QColor ui_foreground_color;
    /// ICON线条笔触
    static QPen ui_icon_pen;

    /// 标题栏宽度（高度）
    static float title_bar_height;

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

    /// 节点名字字体
    static QFont node_name_font;
    /// 节点名字显示位置（相对）
    static QPointF node_name_pos;

    /// 接口之间的间隔
    static float port_interval_size;
    /// 接口的半径
    static float port_radius;
    /// 接口的大小缩放
    static float port_scale;

    /// 连线颜色
    static QColor wire_color;

    /// 背景色
    static QColor editor_background_color;
    /// 前景色透明度
    static QColor editor_foreground_color_a;
    /// 前景色无透明度
    static QColor editor_foreground_color;

public:
    static QColor AverageColor(QColor a, QColor b);

public:
    globalui();
};

} // namespace UserInterface

#endif // GLOBALUI
