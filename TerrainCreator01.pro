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
    Global/globalfunc.cpp \
    Global/globalgl.cpp \
    Global/globalinfo.cpp \
    Global/globalrender.cpp \
    Global/globalui.cpp \
    Kernel/Nodes/Filter/kernel_invert_node.cpp \
    Kernel/Nodes/Filter/kernel_terrace_node.cpp \
    Kernel/Nodes/Math/kernel_number_node.cpp \
    Kernel/Nodes/Nature/kernel_basicerosion01_node.cpp \
    Kernel/Nodes/Texture/kernel_simpleoverlay_node.cpp \
    Kernel/Nodes/Selector/kernel_selectslope_node.cpp \
    Kernel/kernel_element_forwarddec.cpp \
    Kernel/kernel_element_inc.cpp \
    Kernel/Nodes/Filter/kernel_clamp_node.cpp \
    Kernel/Nodes/Generator/kernel_input_node.cpp \
    Kernel/kernel_node.cpp \
    Kernel/kernel_nodegraph.cpp \
    Kernel/Nodes/Generator/kernel_perlinnoise_node.cpp \
    Kernel/Nodes/Texture/kernel_normalmap_node.cpp \
    Kernel/kernel_port.cpp \
    Kernel/kernel_wire.cpp \
    Render/Light/render_light.cpp \
    Render/Light/render_parallellight.cpp \
    Render/render_renderimage2d.cpp \
    Render/render_rendertexture.cpp \
    Render/render_terrainmesh.cpp \
    UserInterface/Inspector/userinterface_nodeinspector_item.cpp \
    UserInterface/NodeMenu/userinterface_nodecreator.cpp \
    UserInterface/NodeMenu/userinterface_nodemenubutton.cpp \
    UserInterface/NodeMenu/userinterface_nodesubmenu.cpp \
    UserInterface/Title/Buttons/userinterface_titlebar_filebutton.cpp \
    UserInterface/Title/userinterface_titlebar_maximizebutton.cpp \
    UserInterface/Title/userinterface_titlebar_minimizbutton.cpp \
    UserInterface/Title/userinterface_titlebar_quitbutton.cpp \
    UserInterface/Title/userinterface_titlebarbutton.cpp \
    UserInterface/userinterface_beginresolvebutton.cpp \
    UserInterface/userinterface_nodeeditorterminal.cpp \
    UserInterface/userinterface_nodegraph.cpp \
    UserInterface/userinterface_node.cpp \
    UserInterface/Inspector/userinterface_nodeinspector.cpp \
    UserInterface/NodeMenu/userinterface_nodemenu.cpp \
    UserInterface/userinterface_port.cpp \
    UserInterface/Title/userinterface_titlebar.cpp \
    UserInterface/userinterface_wire.cpp \
    main.cpp \
    mainwindow.cpp \
    Render/render_camera.cpp \
    Render/renderer.cpp

HEADERS += \
    Global/globalfunc.h \
    Global/globalgl.h \
    Global/globalinfo.h \
    Global/globalrender.h \
    Global/globalui.h \
    Kernel/Nodes/Filter/kernel_invert_node.h \
    Kernel/Nodes/Filter/kernel_terrace_node.h \
    Kernel/Nodes/Math/kernel_number_node.h \
    Kernel/Nodes/Nature/kernel_basicerosion01_node.h \
    Kernel/Nodes/Texture/kernel_simpleoverlay_node.h \
    Kernel/Nodes/Selector/kernel_selectslope_node.h \
    Kernel/kernel_element_inc.h \
    Kernel/Nodes/Filter/kernel_clamp_node.h \
    Kernel/Nodes/Generator/kernel_input_node.h \
    Kernel/kernel_node.h \
    Kernel/kernel_nodegraph.h \
    Kernel/Nodes/Generator/kernel_perlinnoise_node.h \
    Kernel/Nodes/Texture/kernel_normalmap_node.h \
    Kernel/kernel_port.h \
    Kernel/kernel_wire.h \
    Render/Light/render_light.h \
    Render/Light/render_parallellight.h \
    Render/render_renderimage2d.h \
    Render/render_rendertexture.h \
    Render/render_terrainmesh.h \
    UserInterface/Inspector/userinterface_nodeinspector_item.h \
    UserInterface/NodeMenu/userinterface_nodecreator.h \
    UserInterface/NodeMenu/userinterface_nodemenubutton.h \
    UserInterface/NodeMenu/userinterface_nodesubmenu.h \
    UserInterface/Title/Buttons/userinterface_titlebar_filebutton.h \
    UserInterface/Title/userinterface_titlebar_maximizebutton.h \
    UserInterface/Title/userinterface_titlebar_minimizbutton.h \
    UserInterface/Title/userinterface_titlebar_quitbutton.h \
    UserInterface/Title/userinterface_titlebarbutton.h \
    UserInterface/userinterface_beginresolvebutton.h \
    UserInterface/userinterface_nodeeditorterminal.h \
    UserInterface/userinterface_nodegraph.h \
    UserInterface/userinterface_node.h \
    UserInterface/Inspector/userinterface_nodeinspector.h \
    UserInterface/NodeMenu/userinterface_nodemenu.h \
    UserInterface/userinterface_port.h \
    UserInterface/Title/userinterface_titlebar.h \
    UserInterface/userinterface_wire.h \
    mainwindow.h \
    Render/render_camera.h \
    Render/renderer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images/Images.qrc \
    Shaders/RenderShaders.qrc
