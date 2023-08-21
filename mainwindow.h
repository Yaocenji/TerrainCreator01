#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLFunctions_4_5_Core>
#include <QPushButton>
#include <QSplitter>

#include "Global/globalinfo.h"
#include "Kernel/kernel_nodegraph.h"
#include "UserInterface/userinterface_nodeeditorterminal.h"
#include "UserInterface/userinterface_nodegraph.h"
#include "renderer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    int width = 1920;
    int height = 1080;

protected:
    QSplitter *splt;
    Renderer *renderWidget;
    UserInterface::NodeEditorTerminal *uiNodeEditor;
    Kernel::PerlinNoise_Node *pn;
    QPushButton *pb1;
public slots:
    void test();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
