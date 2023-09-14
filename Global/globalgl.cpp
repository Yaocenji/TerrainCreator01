#include "globalgl.h"

globalgl::globalgl() {
}

QOpenGLFunctions_4_5_Core *globalgl::thisContext = nullptr;

bool globalgl::isDebuggingGL = false;
