#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H

#include <QColor>
#include <QDebug>
#include <QtMath>

class globalfunc {
public:
    globalfunc();
};

// 一些全局函数
// 钳制
template <typename T>
T clamp(T l, T r, T x) {
    T L, R;
    if (l > r) {
        L = r;
        R = l;
    } else {
        L = l;
        R = r;
    }
    if (x > R)
        return R;
    else if (x < L) {
        return L;
    } else {
        return x;
    }
}

/// 平均色
QColor AverageColor(QColor a, QColor b);
/// 颜色乘数1
QColor MultipyColor(QColor a, float f);
/// 颜色乘数2
QColor MultipyColor(QColor a, float f, int chan);

#endif // GLOBALFUNC_H
