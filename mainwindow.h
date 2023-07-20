#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLFunctions_4_5_Core>
#include <QPushButton>

#include "globalinfo.h"
#include "kernel_nodegraph.h"
#include "renderer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    int width = 1920;
    int height = 1080;

protected:
    Renderer *renderWidget;
    kernel::Kernel_PerlinNoise_Node *pn;
    QPushButton *pb1;
public slots:
    void test();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
