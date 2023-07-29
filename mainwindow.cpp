#include "mainwindow.h"

#include <QOpenGLContext>

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
    pn = new kernel::Kernel_PerlinNoise_Node(this);
    pn->InitGL(*renderWidget->getFunctionAndContext());
    pn->Allocate(*renderWidget->getFunctionAndContext());
    pn->RunNode(*renderWidget->getFunctionAndContext());
    pn->Choose(*renderWidget->getFunctionAndContext());
    qDebug() << "成功创建";
    globalinfo::useHeightFieldBuffer = true;
    renderWidget->ChosenHeightFieldBuffer = pn->OutputPorts[0]->GetBufferData();
    renderWidget->update();
}

MainWindow::~MainWindow() {
}
