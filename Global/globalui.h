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
    /// 节点被选中效果颜色
    static QColor node_chosen_color;

    /// 接口之间的间隔
    static float port_interval_size;
    /// 接口的半径
    static float port_radius;
    /// 接口的大小缩放
    static float port_scale;

    /// 连线颜色
    static QColor wire_color;

    /// 节点菜单左侧一级菜单宽度
    static float node_menu_button_width;
    /// 节点菜单左侧按钮高度
    static float node_menu_button_height;
    /// 节点菜单按钮高度
    static float node_menu_creator_height;
    /// 节点菜单字体（标题）
    static QFont node_menu_title_font;
    /// 节点菜单字体（描述）
    static QFont node_menu_descrip_font;

    /// 节点参数编辑器参数名字体
    static QFont node_inspector_name_font;
    /// 节点参数编辑器条目宽高
    static QPointF node_inspector_item_size;
    /// 节点参数编辑器条目上边距
    static float node_inspector_item_topmargin;
    /// 节点参数编辑器输入框大小
    static QPointF node_inspector_lineedit_size;

    /// 背景色
    static QColor editor_background_color;
    /// 前景色透明度
    static QColor editor_foreground_color_a;
    /// 前景色无透明度
    static QColor editor_foreground_color;

public:
public:
    globalui();
};

} // namespace UserInterface

/// 颜色判等

#endif // GLOBALUI
