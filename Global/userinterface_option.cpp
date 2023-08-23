#include "userinterface_option.h"

namespace UserInterface {

Option::Option() {
}

float Option::input_column_found_size[2] = {35, 35};
float Option::output_column_found_size[2] = {35, 35};
float Option::param_line_found_size[2] = {110, 35};
float Option::node_scale = 1.0f;
float Option::node_angle_radius = 8.0f;
float Option::pen_width = 3.0;

QFont Option::node_name_font = QFont("微软雅黑", 14);
QPointF Option::node_name_pos = QPointF(0.0, -4.0);

float Option::port_interval_size = 30;
float Option::port_scale = 1.0f;
float Option::port_radius = 7.0f;

QColor Option::background_color = QColor(41, 43, 49);
QColor Option::foreground_color = QColor(229, 227, 205, 50);

} // namespace UserInterface
