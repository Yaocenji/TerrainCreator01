#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLFunctions_4_5_Core>
#include <QPushButton>
#include <QSplitter>

#include "Global/globalinfo.h"
#include "Global/globalui.h"
#include "Kernel/kernel_element_inc.h"
#include "UserInterface/NodeMenu/userinterface_nodemenu.h"
#include "UserInterface/Title/userinterface_titlebar.h"
#include "UserInterface/userinterface_beginresolvebutton.h"
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
    /// 抬头栏与主窗口分割器
    QSplitter *titleBarSplt;
    /// 上下分割器
    QSplitter *splt;
    /// 下半部分：左右分割器
    QSplitter *splt_1;

    /// 标题栏
    UserInterface::TitleBar *titleBar;

    /// 渲染窗口
    Renderer *renderWidget;
    /// 开始解算按钮
    UserInterface::BeginResolveButton *beginResolveBtn;
    /// 节点编辑器
    UserInterface::NodeEditorTerminal *uiNodeEditor;
    /// 运行按钮
    QPushButton *pb1;
    /// kernel与ui节点图
    Kernel::NodeGraph *kNG;
    UserInterface::NodeGraph *uiNG;
    /// 节点创建菜单
    UserInterface::NodeMenu *nodeMenu;

public slots:
    void test();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
