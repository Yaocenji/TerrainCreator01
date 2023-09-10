#include "userinterface_nodeinspector_item.h"

#include "Global/globalfunc.h"
#include "Global/globalui.h"

namespace UserInterface {

NodeInspectorItem::NodeInspectorItem(QWidget *parent) : QWidget(parent) {
    name = "测试";
    resize(200, 30);
    targetKernelNode = nullptr;
    targetParamType = ItemTargetParamType::PortParam;
    index = 0;

    lable = nullptr;

    lineEdit = nullptr;
    slider = nullptr;
    comboBox = nullptr;

    // 启动计时器
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(33);
    timer->start();
    // 根据计时器信号更新画面
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
}

void NodeInspectorItem::TimerUpdate() {
    static QRect thisRect = rect();
    if (thisRect != rect()) {
        ResetControllerGeometry();
        thisRect = rect();
    }
}

int NodeInspectorItem::GetTargetParam(unsigned int &buffer, float &f, int &i,
                                      int &e, QColor &c) {
    if (targetKernelNode == nullptr) {
        qDebug() << "错误：item获取目标数据错误：目标节点为空指针";
        return -1;
    }
    if (targetParamType == ItemTargetParamType::PortParam) {
        if (index >= targetKernelNode->ParamPorts.size() || index < 0) {
            qDebug() << "错误：item获取目标数据错误：index索引越界";
            return -1;
        }
        Kernel::Port *tarPort = targetKernelNode->ParamPorts[index];
        unsigned int data_buf = 0;
        float data_fl = 0;
        int flag = tarPort->GetData(data_buf, data_fl);
        if (flag == 0) {
            buffer = data_buf;
            return 0;
        } else if (flag == 1) {
            f = data_fl;
            return 1;
        } else {
            qDebug() << "错误：item获取目标数据错误：在目标port中获取目标参数时"
                        "返回值错误";
            return -1;
        }
    } else {
        if (index >= targetKernelNode->nonPortParams.size() || index < 0) {
            qDebug() << "错误：item获取目标数据错误：index索引越界";
            return -1;
        }
        Kernel::NonPortParam *tarNonPortParam =
            targetKernelNode->nonPortParams[index];
        float data_fl = 0;
        int data_in = 0;
        int data_enu = 0;
        QColor data_col = QColor();
        int flag =
            tarNonPortParam->GetData(data_fl, data_in, data_enu, data_col);
        if (flag < 0 || flag > 3) {
            qDebug() << "错误：item获取目标数据错误：在目标NonPortParam中获取目"
                        "标参数时"
                        "返回值错误";
            return -1;
        }
        if (flag == 0) {
            f = data_fl;
        } else if (flag == 1) {
            i = data_in;
        } else if (flag == 2) {
            e = data_enu;
        } else {
            c = data_col;
        }
        return flag + 1;
    }
}

void NodeInspectorItem::DeleteAllController() {
    if (lineEdit != nullptr) {
        delete lineEdit;
        lineEdit = nullptr;
    }
    if (slider != nullptr) {
        delete slider;
        slider = nullptr;
    }
    if (comboBox != nullptr) {
        delete comboBox;
        comboBox = nullptr;
    }
}

bool NodeInspectorItem::SetTargetParamType(Kernel::Node *tarNode,
                                           ItemTargetParamType type, int ind) {
    targetKernelNode = tarNode;
    targetParamType = type;
    index = ind;

    DeleteAllController();

    if (type == ItemTargetParamType::PortParam) {
        if (index >= targetKernelNode->ParamPorts.size() || index < 0) {
            qDebug() << "错误：item指定目标数据错误：index索引越界";
            return false;
        }
        Kernel::Port *tarPort = targetKernelNode->ParamPorts[index];
        name = tarPort->name;

        lable = new QLabel(name, this);

        unsigned int data_buf = 0;
        float data_fl = 0;
        int flag = tarPort->GetData(data_buf, data_fl);
        if (flag == 0) {
            // TODO 当该paramPort的数据类型为buffer时
            return false;
        } else if (flag == 1) {
            // 当目标是一个paramPort的float参数时
            // 生成一个inputDialog
            lineEdit = new QLineEdit(this);
            lineEdit->setText(QString::number(data_fl));
            // 连接：lineEdit被修改时
            connect(lineEdit, SIGNAL(editingFinished()), this,
                    SLOT(LineEditFinished()));

            return true;
        } else {
            qDebug() << "错误：item获取目标数据错误：在目标port中获取目标参数时"
                        "返回值错误";
            return false;
        }
    } else {
        if (index >= targetKernelNode->nonPortParams.size() || index < 0) {
            qDebug() << "错误：item获取目标数据错误：index索引越界";
            return -1;
        }
        Kernel::NonPortParam *tarNonPortParam =
            targetKernelNode->nonPortParams[index];
        name = tarNonPortParam->name;

        lable = new QLabel(name, this);

        if (tarNonPortParam->type == Kernel::NonPortParamType::Float) {
            lineEdit = new QLineEdit(this);
            lineEdit->setText(QString::number(tarNonPortParam->data_float));
            // 连接：lineEdit被修改时
            connect(lineEdit, SIGNAL(editingFinished()), this,
                    SLOT(LineEditFinished()));

        } else if (tarNonPortParam->type ==
                   Kernel::NonPortParamType::RangeFloat) {
            slider = new QSlider(this);
            slider->setOrientation(Qt::Horizontal); // 水平方向
            slider->setMinimum(tarNonPortParam->rangefloat_l *
                               100.0f); // 最小值
            slider->setMaximum(tarNonPortParam->rangefloat_r *
                               100.0f);                   // 最大值
            slider->setSingleStep(1);                     // 步长
            slider->setTickInterval(10);                  // 设置刻度间隔
            slider->setTickPosition(QSlider::TicksAbove); // 刻度在上方

            lineEdit = new QLineEdit(this);

            lineEdit->setText(
                QString::number(tarNonPortParam->data_rangefloat));
            // 连接：lineEdit被修改时
            connect(lineEdit, SIGNAL(editingFinished()), this,
                    SLOT(LineEditFinished()));
            // 连接：slider被修改时
            connect(slider, SIGNAL(valueChanged(int)), this,
                    SLOT(SliderSlided(int)));
        } else if (tarNonPortParam->type == Kernel::NonPortParamType::Int) {
            lineEdit = new QLineEdit(this);

            lineEdit->setText(QString::number(tarNonPortParam->data_int));
            // 连接：lineEdit被修改时
            connect(lineEdit, SIGNAL(editingFinished()), this,
                    SLOT(LineEditFinished()));
        } else if (tarNonPortParam->type ==
                   Kernel::NonPortParamType::RangeInt) {
            slider = new QSlider(this);
            slider->setOrientation(Qt::Horizontal);          // 水平方向
            slider->setMinimum(tarNonPortParam->rangeint_l); // 最小值
            slider->setMaximum(tarNonPortParam->rangeint_r); // 最大值
            slider->setSingleStep(1);                        // 步长
            slider->setTickInterval(10);                  // 设置刻度间隔
            slider->setTickPosition(QSlider::TicksAbove); // 刻度在上方

            lineEdit = new QLineEdit(this);
            lineEdit->setText(QString::number(tarNonPortParam->data_rangeint));

            // 连接：lineEdit被修改时
            connect(lineEdit, SIGNAL(editingFinished()), this,
                    SLOT(LineEditFinished()));
            // 连接：slider被修改时
            connect(slider, SIGNAL(valueChanged(int)), this,
                    SLOT(SliderSlided(int)));
        } else if (tarNonPortParam->type == Kernel::NonPortParamType::Enum) {
            comboBox = new QComboBox(this);
            comboBox->move(60, 0);
            comboBox->resize(rect().width() - 60, rect().height());
        }
        return true;
    }
}

void NodeInspectorItem::ResetControllerGeometry() {
    if (lable != nullptr) {
        lable->setGeometry(rect());
    }
    if (lineEdit != nullptr) {
        lineEdit->setGeometry(
            rect().width() - globalui::node_inspector_lineedit_size.x(),
            rect().center().y() -
                globalui::node_inspector_lineedit_size.y() / 2.0,
            globalui::node_inspector_lineedit_size.x(),
            globalui::node_inspector_lineedit_size.y());
    }
    if (slider != nullptr) {
        slider->setGeometry(
            rect().center().x(),
            rect().center().y() -
                globalui::node_inspector_lineedit_size.y() / 2.0,
            globalui::node_inspector_lineedit_size.x(),
            globalui::node_inspector_lineedit_size.y());
    }
    if (comboBox != nullptr) {
        comboBox->setGeometry(
            rect().width() - globalui::node_inspector_lineedit_size.x(),
            rect().center().y() -
                globalui::node_inspector_lineedit_size.y() / 2.0,
            globalui::node_inspector_lineedit_size.x(),
            globalui::node_inspector_lineedit_size.y());
    }
}

void NodeInspectorItem::LineEditFinished() {
    qDebug() << "触发函数：修改lineEdit";
    QString text = lineEdit->text();
    if (targetParamType == ItemTargetParamType::PortParam) {
        Kernel::Port *tarPort = targetKernelNode->ParamPorts[index];
        if (tarPort->GetDataType() == Kernel::PortDataType::Float) {
            bool flag;
            float data = text.toFloat(&flag);
            if (flag) {
                tarPort->SetDefaultFloatData(data);
            }

            lineEdit->setText(QString::number(tarPort->GetDefaultFloatData()));

            targetKernelNode->OccurChangeOnPort(tarPort);
            return;
        } else {
            // TODO
            return;
        }
    } else {
        Kernel::NonPortParam *tarNonPortParam =
            targetKernelNode->nonPortParams[index];
        if (tarNonPortParam->type == Kernel::NonPortParamType::Float ||
            tarNonPortParam->type == Kernel::NonPortParamType::RangeFloat) {
            bool flag;
            float data = text.toFloat(&flag);
            if (flag) {
                tarNonPortParam->SetData(data);
            }
            lineEdit->setText(QString::number(tarNonPortParam->data_float));
            if (tarNonPortParam->type == Kernel::NonPortParamType::RangeFloat) {
                lineEdit->setText(
                    QString::number(tarNonPortParam->data_rangefloat));
                slider->setValue(
                    int(tarNonPortParam->data_rangefloat * 100.0f));
            }

            targetKernelNode->OccurChangeOnNonPortParam(tarNonPortParam);
            return;
        } else if (tarNonPortParam->type == Kernel::NonPortParamType::Int ||
                   tarNonPortParam->type ==
                       Kernel::NonPortParamType::RangeInt) {
            bool flag;
            int data = int(text.toFloat(&flag));
            if (flag) {
                tarNonPortParam->SetData(data);
            }
            lineEdit->setText(QString::number(tarNonPortParam->data_int));
            if (tarNonPortParam->type == Kernel::NonPortParamType::RangeInt) {
                lineEdit->setText(
                    QString::number(tarNonPortParam->data_rangeint));
                slider->setValue(tarNonPortParam->data_rangeint);
            }

            targetKernelNode->OccurChangeOnNonPortParam(tarNonPortParam);
            return;

        } else {
            qDebug() << "错误 在类型参数不为数时，lineEdit不应该被修改";
            return;
        }
    }
}

void NodeInspectorItem::SliderSlided(int value) {
    qDebug() << "触发函数：滑动滑动条";
    if (targetParamType == ItemTargetParamType::PortParam) {
        Kernel::Port *tarPort = targetKernelNode->ParamPorts[index];
        if (tarPort->GetDataType() == Kernel::PortDataType::Float) {
            bool flag;
            float data = value;
            tarPort->SetDefaultFloatData(data);

            lineEdit->setText(QString::number(tarPort->GetDefaultFloatData()));
            targetKernelNode->OccurChangeOnPort(tarPort);
            return;
        } else {
            // TODO
            return;
        }
    } else {
        Kernel::NonPortParam *tarNonPortParam =
            targetKernelNode->nonPortParams[index];
        if (tarNonPortParam->type == Kernel::NonPortParamType::RangeFloat) {
            float data = (float(value)) / 100.0f;
            tarNonPortParam->SetData(data);
            lineEdit->setText(
                QString::number(tarNonPortParam->data_rangefloat));
            slider->setValue(int(tarNonPortParam->data_rangefloat * 100.0f));

            targetKernelNode->OccurChangeOnNonPortParam(tarNonPortParam);
            return;
        } else if (tarNonPortParam->type ==
                   Kernel::NonPortParamType::RangeInt) {
            int data = value;
            tarNonPortParam->SetData(data);

            lineEdit->setText(QString::number(tarNonPortParam->data_rangeint));
            slider->setValue(tarNonPortParam->data_rangeint);

            targetKernelNode->OccurChangeOnNonPortParam(tarNonPortParam);
            return;

        } else {
            qDebug() << "错误 在类型参数不为数时，lineEdit不应该被修改";
            return;
        }
    }
}

void NodeInspectorItem::paintEvent(QPaintEvent *event) {
    //    Q_UNUSED(event);

    //    QPainter p;
    //    p.begin(this);
    //    // 反走样
    //    p.setRenderHint(QPainter::Antialiasing, true);

    //    p.setFont(globalui::node_inspector_name_font);
    //    p.setPen(QPen(Qt::black));

    //    p.drawText(QPointF(0, rect().height()), name);

    //    p.drawRect(rect());
}

} // namespace UserInterface
