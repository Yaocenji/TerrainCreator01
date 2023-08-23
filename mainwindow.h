#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLFunctions_4_5_Core>
#include <QPushButton>
#include <QSplitter>

#include "Global/globalinfo.h"
#include "Kernel/kernel_element_inc.h"
#include "UserInterface/userinterface_nodeeditorterminal.h"
#include "UserInterface/userinterface_nodegraph.h"
#include "renderer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    int width = 1920;
    int height = 1080;

protected:
    /// 窗口分割器
    QSplitter *splt;
    QSplitter *splt_1;
    /// 渲染窗口
    Renderer *renderWidget;
    /// 节点编辑器
    UserInterface::NodeEditorTerminal *uiNodeEditor;
    /// 运行按钮
    QPushButton *pb1;
    /// kernel与ui节点图
    Kernel::NodeGraph *kNG;
    UserInterface::NodeGraph *uiNG;
public slots:
    void test();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
