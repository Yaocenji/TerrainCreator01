#include "mainwindow.h"

#include <QOpenGLContext>
#include <type_traits>

#include "Kernel/kernel_element_inc.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(width, height);

    renderWidget = new Renderer(this);

    kNG = new Kernel::NodeGraph(this, false, nullptr);
    uiNG = new UserInterface::NodeGraph(this, kNG);

    uiNodeEditor = new UserInterface::NodeEditorTerminal(this);

    uiNodeEditor->SetTargetNodeGraph(uiNG);

    uiNG->addNode<Kernel::PerlinNoise_Node>(QPointF(150, 150));
    uiNG->addNode<Kernel::PerlinNoise_Node>(QPointF(150, 150));
    uiNG->addNode<Kernel::PerlinNoise_Node>(QPointF(150, 150));
    uiNG->addNode<Kernel::Clamp_Node>(QPointF(550, 170));
    uiNG->addNode<Kernel::Clamp_Node>(QPointF(550, 170));
    uiNG->addNode<Kernel::Clamp_Node>(QPointF(550, 170));
    //    uiNG->LinkWire(uiNG->GetNodes()[0]->OutputPorts[0],
    //                   uiNG->GetNodes()[1]->InputPorts[0]);

    pb1 = new QPushButton(this);
    pb1->setText("开始计算");

    splt = new QSplitter(this);
    splt->addWidget(renderWidget);

    splt_1 = new QSplitter(Qt::Vertical, this);

    splt_1->addWidget(uiNodeEditor);
    splt_1->addWidget(pb1);

    splt_1->setStretchFactor(0, 80);
    splt_1->setStretchFactor(1, 20);

    splt->addWidget(splt_1);

    splt->setStretchFactor(0, 65);
    splt->setStretchFactor(1, 35);

    splt->show();
    setCentralWidget(splt);

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
