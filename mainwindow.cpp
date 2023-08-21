#include "mainwindow.h"

#include <QOpenGLContext>
#include <type_traits>

#include "Kernel/kernel_clamp_node.h"
#include "Kernel/kernel_perlinnoise_node.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(width, height);

    renderWidget = new Renderer(this);
    //    renderWidget->show();
    //    setCentralWidget(renderWidget);
    Kernel::NodeGraph *kNG = new Kernel::NodeGraph(this, false, nullptr);
    UserInterface::NodeGraph *uiNG = new UserInterface::NodeGraph(this, kNG);

    uiNodeEditor = new UserInterface::NodeEditorTerminal(this);

    uiNodeEditor->SetTargetNodeGraph(uiNG);

    uiNG->addNode<Kernel::PerlinNoise_Node>(QPointF(150, 150));

    splt = new QSplitter(this);
    splt->addWidget(renderWidget);
    splt->addWidget(uiNodeEditor);

    splt->show();
    setCentralWidget(splt);

    pb1 = new QPushButton(this);
    pb1->setText("test");

    connect(pb1, SIGNAL(clicked()), this, SLOT(test()));
}

void MainWindow::test() {
    pn = new Kernel::PerlinNoise_Node(this);
    //    pn->InitGL(*renderWidget->getFunctionAndContext());
    //    pn->Allocate(*renderWidget->getFunctionAndContext());
    //    pn->RunNode(*renderWidget->getFunctionAndContext());
    //    pn->Choose(*renderWidget->getFunctionAndContext());

    Kernel::Clamp_Node *Cnn = new Kernel::Clamp_Node(this);
    //    cn->InitGL(*renderWidget->getFunctionAndContext());
    //    cn->Allocate(*renderWidget->getFunctionAndContext());
    //    cn->RunNode(*renderWidget->getFunctionAndContext());
    //    cn->Choose(*renderWidget->getFunctionAndContext());

    Kernel::NodeGraph *ng;

    ng = new Kernel::NodeGraph(this);
    ng->AddNode(pn);
    ng->AddNode(Cnn);

    qDebug() << renderWidget->getFunctionAndContext()->glGetError();

    ng->LinkWire(ng->nodes[0]->OutputPorts[0], ng->nodes[1]->InputPorts[0]);
    ng->RunNodeGraph(*renderWidget->getFunctionAndContext());

    qDebug() << renderWidget->getFunctionAndContext()->glGetError();

    qDebug() << "成功创建";

    globalinfo::useHeightFieldBuffer = true;

    globalinfo::ChosenHeightFieldBuffer =
        ng->nodes[1]->OutputPorts[0]->GetBufferData();

    renderWidget->update();

    qDebug() << std::is_base_of<QObject, QWidget>::value;

    //  调试交互编辑器
}

MainWindow::~MainWindow() {
}
