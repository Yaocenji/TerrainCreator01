#ifndef NODEINSPECTORITEM_H
#define NODEINSPECTORITEM_H

#include <QComboBox>
#include <QDebug>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QSlider>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "Kernel/kernel_element_inc.h"

namespace UserInterface {

enum class ItemTargetParamType { PortParam, NonPortParam };

/// 节点参数编辑器中的“一条”
class NodeInspectorItem : public QWidget {
    Q_OBJECT

protected:
    QTimer *timer;
protected slots:
    void TimerUpdate();

public:
    /// 显示名称
    QString name;
    /// 目标（kernel）节点
    Kernel::Node *targetKernelNode;
    /// 类型：接口参数与非接口参数
    ItemTargetParamType targetParamType;
    /// 索引：目标参数在参数接口/非接口参数数组中的索引
    int index;

public:
    /// 获取目标参数
    int GetTargetParam(unsigned int &buffer, float &f, int &i, int &e,
                       QColor &c);
    /// 清除所有的控制器内存
    void DeleteAllController();

    /// 指定一个目标参数——重载1
    bool SetTargetParamType(Kernel::Node *tarNode, ItemTargetParamType type,
                            int ind);

    /// 重设控件位置
    void ResetControllerGeometry();

public:
    /// 名字显示器
    QLabel *lable;
    /// 可能的控制器
    /// 一个输入框
    QLineEdit *lineEdit;
    /// 一个滑动条（滑块值只能取整数，在控制范围小数时，将整数数倍缩小作为定点小数）
    QSlider *slider;
    /// 一个下拉菜单
    QComboBox *comboBox;

public slots:
    /// lineEdit编辑完成后
    void LineEditFinished();
    /// 滑动了滑动条
    void SliderSlided(int value);

public:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit NodeInspectorItem(QWidget *parent = nullptr);

signals:
};

} // namespace UserInterface

#endif // NODEINSPECTORITEM_H
