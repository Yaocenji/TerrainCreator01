#include "mainwindow.h"

#include <QOpenGLContext>
#include <type_traits>

#include "Kernel/kernel_element_inc.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(width, height);

    // 最大化
    //    showMaximized();
    // 隐藏标题栏
    setWindowFlags(Qt::FramelessWindowHint);

    titleBar = new UserInterface::TitleBar(this, this);

    renderWidget = new Renderer(this);

    kNG = new Kernel::NodeGraph(this, false, nullptr);
    uiNG = new UserInterface::NodeGraph(this, kNG);

    uiNodeEditor = new UserInterface::NodeEditorTerminal(this);

    uiNodeEditor->SetTargetNodeGraph(uiNG);

    uiNG->addNode<Kernel::PerlinNoise_Node>(QPointF(150, 150));
    uiNG->addNode<Kernel::Clamp_Node>(QPointF(550, 170));
    //    uiNG->LinkWire(uiNG->GetNodes()[0]->OutputPorts[0],
    //                   uiNG->GetNodes()[1]->InputPorts[0]);

    pb1 = new QPushButton(this);
    pb1->setText("开始计算");

    // 分割器初始化
    // 创建下半部分分割器
    splt_1 = new QSplitter(Qt::Horizontal, this);
    splt_1->setHandleWidth(5);

    splt_1->addWidget(uiNodeEditor);
    splt_1->addWidget(pb1);

    splt_1->setStretchFactor(0, 80);
    splt_1->setStretchFactor(1, 20);

    // 创建主分割器
    splt = new QSplitter(Qt::Vertical, this);
    splt->addWidget(renderWidget);
    splt->setHandleWidth(5);

    splt->addWidget(splt_1);

    splt->setStretchFactor(0, 65);
    splt->setStretchFactor(1, 35);

    // 创建标题栏分割器
    titleBarSplt = new QSplitter(Qt::Vertical, this);
    titleBarSplt->setHandleWidth(0);

    titleBarSplt->addWidget(titleBar);
    titleBarSplt->addWidget(splt);

    titleBarSplt->setStretchFactor(0, 0);
    titleBarSplt->setStretchFactor(1, 1);

    // 主分割器设置大小
    QList<int> sizelist;
    sizelist.push_back(UserInterface::globalui::title_bar_height);
    sizelist.push_back(rect().height() -
                       UserInterface::globalui::title_bar_height);

    titleBarSplt->setSizes(sizelist);
    // 主分割器不可调整
    titleBarSplt->handle(1)->setDisabled(true);

    titleBarSplt->show();
    setCentralWidget(titleBarSplt);

    beginResolveBtn = new UserInterface::BeginResolveButton(renderWidget);
    beginResolveBtn->show();

    connect(pb1, SIGNAL(clicked()), this, SLOT(test()));
}

void MainWindow::test() {
    //  调试交互编辑器

    // 试运行
    kNG->RunNodeGraph(*renderWidget->getFunctionAndContext());
    globalinfo::useHeightFieldBuffer = true;
    globalinfo::ChosenHeightFieldBuffer =
        kNG->nodes[1]->OutputPorts[0]->GetBufferData();
    renderWidget->update();
}

MainWindow::~MainWindow() {
}
