#include "mainwindow.h"

#include <QOpenGLContext>

#include "Kernel/kernel_clamp_node.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(width, height);

    renderWidget = new Renderer();
    renderWidget->show();
    setCentralWidget(renderWidget);

    pb1 = new QPushButton(this);
    pb1->setText("test");

    connect(pb1, SIGNAL(clicked()), this, SLOT(test()));
}

void MainWindow::test() {
    pn = new kernel::PerlinNoise_Node(this);
    //    pn->InitGL(*renderWidget->getFunctionAndContext());
    //    pn->Allocate(*renderWidget->getFunctionAndContext());
    //    pn->RunNode(*renderWidget->getFunctionAndContext());
    //    pn->Choose(*renderWidget->getFunctionAndContext());

    kernel::Clamp_Node *cn;
    cn = new kernel::Clamp_Node(this);
    //    cn->InitGL(*renderWidget->getFunctionAndContext());
    //    cn->Allocate(*renderWidget->getFunctionAndContext());
    //    cn->RunNode(*renderWidget->getFunctionAndContext());
    //    cn->Choose(*renderWidget->getFunctionAndContext());

    kernel::NodeGraph *ng;

    ng = new kernel::NodeGraph(this);
    ng->AddNode(pn);
    ng->AddNode(cn);

    qDebug() << renderWidget->getFunctionAndContext()->glGetError();

    ng->LinkWire(ng->nodes[0]->OutputPorts[0], ng->nodes[1]->InputPorts[0]);
    ng->RunNodeGraph(*renderWidget->getFunctionAndContext());

    qDebug() << renderWidget->getFunctionAndContext()->glGetError();

    qDebug() << "成功创建";

    globalinfo::useHeightFieldBuffer = true;

    renderWidget->update();
}

MainWindow::~MainWindow() {
}
