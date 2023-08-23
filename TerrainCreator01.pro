QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Global/globalinfo.cpp \
    Global/userinterface_option.cpp \
    Kernel/kernel_element_forwarddec.cpp \
    Kernel/kernel_element_inc.cpp \
    UserInterface/userinterface_nodeeditorterminal.cpp \
    UserInterface/userinterface_nodegraph.cpp \
    UserInterface/userinterface_node.cpp \
    Kernel/Nodes/Filter/kernel_clamp_node.cpp \
    Kernel/Nodes/Generator/kernel_input_node.cpp \
    Kernel/kernel_node.cpp \
    Kernel/kernel_nodegraph.cpp \
    Kernel/Nodes/Generator/kernel_perlinnoise_node.cpp \
    Kernel/kernel_port.cpp \
    Kernel/kernel_wire.cpp \
    UserInterface/userinterface_port.cpp \
    UserInterface/userinterface_wire.cpp \
    main.cpp \
    mainwindow.cpp \
    renderer.cpp

HEADERS += \
    Global/globalinfo.h \
    Global/userinterface_option.h \
    Kernel/kernel_element_inc.h \
    UserInterface/userinterface_nodeeditorterminal.h \
    UserInterface/userinterface_nodegraph.h \
    UserInterface/userinterface_node.h \
    Kernel/Nodes/Filter/kernel_clamp_node.h \
    Kernel/Nodes/Generator/kernel_input_node.h \
    Kernel/kernel_node.h \
    Kernel/kernel_nodegraph.h \
    Kernel/Nodes/Generator/kernel_perlinnoise_node.h \
    Kernel/kernel_port.h \
    Kernel/kernel_wire.h \
    UserInterface/userinterface_port.h \
    UserInterface/userinterface_wire.h \
    mainwindow.h \
    renderer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Shaders/RenderSahders.qrc
