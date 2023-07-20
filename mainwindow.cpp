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
    pn->RunNode(*renderWidget->getFunctionAndContext());
    pn->Choose(*renderWidget->getFunctionAndContext());
    qDebug() << "成功创建";
    // delete pn;
    //    unsigned int data = pn->OutputPorts[0]->GetBufferData();
    //    QOpenGLFunctions_4_5_Core f;
    //    f.initializeOpenGLFunctions();
    //    f.glBindImageTexture(0, data, 8, GL_FALSE, 0, GL_READ_WRITE,
    //    GL_RGBA32F);
    globalinfo::useHeightFieldBuffer = true;
    renderWidget->test = pn->OutputPorts[0]->GetBufferData();
    renderWidget->update();
}

MainWindow::~MainWindow() {
}
