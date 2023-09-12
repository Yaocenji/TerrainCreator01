#ifndef GLOBALRENDER_H
#define GLOBALRENDER_H

#include <QColor>

class globalrender {
public:
    /// 背景色
    static QColor backgroundColor;

    /// 是否实时更新渲染窗口
    static bool real_time_render;

public:
    globalrender();
};

#endif // GLOBALRENDER_H
