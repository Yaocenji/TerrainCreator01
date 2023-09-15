#include "globalui.h"

#include <QDebug>

namespace UserInterface {

globalui::globalui() {
}

bool globalui::real_time_update = false;

float globalui::pen_width = 3.0;

QColor globalui::transparent_color = QColor(0, 0, 0, 0);

QFont globalui::ui_font = QFont("微软雅黑", 10);
float globalui::ui_angle_radius = 8.0f;
QColor globalui::ui_background_color = QColor(61, 63, 69);
QColor globalui::ui_foreground_color = QColor(114, 115, 116);
QPen globalui::ui_icon_pen =
    QPen(QColor(globalui::ui_foreground_color.red() * 0.25 + 192,
                globalui::ui_foreground_color.green() * 0.25 + 192,
                globalui::ui_foreground_color.blue() * 0.25 + 192),
         1);

float globalui::title_bar_height = 40.0f;

float globalui::input_column_found_size[2] = {35, 35};
float globalui::output_column_found_size[2] = {35, 35};
float globalui::param_line_found_size[2] = {110, 35};
float globalui::node_scale = 1.0f;
float globalui::node_angle_radius = 8.0f;

QFont globalui::node_name_font = QFont("微软雅黑", 14);
QPointF globalui::node_name_pos = QPointF(0.0, -10.0);
QColor globalui::node_chosen_color = QColor(255, 255, 122);
QColor globalui::node_locked_color = QColor(255, 39, 0);

float globalui::port_interval_size = 30;
float globalui::port_scale = 1.0f;
float globalui::port_radius = 7.0f;

QColor globalui::wire_color = QColor(210, 210, 210);

float globalui::node_menu_button_width = 65;
float globalui::node_menu_button_height = 65;
float globalui::node_menu_creator_height = 90;
QFont globalui::node_menu_title_font = QFont("微软雅黑", 14);
QFont globalui::node_menu_descrip_font = QFont("微软雅黑", 7);

QFont globalui::node_inspector_name_font = QFont("微软雅黑", 12);
QPointF globalui::node_inspector_item_size = QPointF(500, 40);
float globalui::node_inspector_item_topmargin = 60;
QPointF globalui::node_inspector_lineedit_size = QPointF(40, 20);

QColor globalui::editor_background_color = QColor(41, 43, 49);
QColor globalui::editor_foreground_color_a = QColor(229, 227, 205, 50);
QColor globalui::editor_foreground_color = QColor(114, 113, 112);

} // namespace UserInterface
