#include "globalfunc.h"

globalfunc::globalfunc() {
}

QColor AverageColor(QColor a, QColor b) {
    QColor ans((a.red() + b.red()) / 2.0f, (a.green() + b.green()) / 2.0f,
               (a.blue() + b.blue()) / 2.0f, (a.alpha() + b.alpha()) / 2.0f);
    return ans;
}

QColor MultipyColor(QColor a, float f) {
    QColor ans(a.red() * f, a.green() * f, a.blue() * f, a.alpha());
    return ans;
}

QColor MultipyColor(QColor a, float f, int chan) {
    QColor ans = a;
    if (chan == 0) {
        ans = QColor(a.red() * f, a.green(), a.blue(), a.alpha());
    } else if (chan == 1) {
        ans = QColor(a.red(), a.green() * f, a.blue(), a.alpha());
    } else if (chan == 2) {
        ans = QColor(a.red(), a.green(), a.blue() * f, a.alpha());
    } else if (chan == 3) {
        ans = QColor(a.red(), a.green(), a.blue(), a.alpha() * f);
    } else {
        qDebug() << "ERROR：MultipyColor函数，参数错误";
    }
    return ans;
}
