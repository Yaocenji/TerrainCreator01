#include "globalrender.h"

globalrender::globalrender() {
}

QColor globalrender::backgroundColor = QColor(0, 0, 0);

bool globalrender::real_time_render = true;
