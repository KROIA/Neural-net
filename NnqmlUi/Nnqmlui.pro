QT += charts qml quick sql
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
netIncPath = ../inc
netSrcPath = ../src

INCLUDEPATH += $$netIncPath \
               $$inc

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    $$netSrcPath/net.cpp \
    $$netSrcPath/neuron.cpp \
    $$netSrcPath/activation.cpp \
    $$netSrcPath/backpropnet.cpp \
    $$netSrcPath/geneticnet.cpp \
    $$netSrcPath/savenet.cpp \
    netdatabase.cpp \
    thread.cpp \
    daten.cpp \
    datenset.cpp

RESOURCES += qml.qrc



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target qml

DISTFILES += \
    qml/Connection.qml \
    qml/ErrorChart.qml \
    qml/NetClassic.qml \
    qml/NetInCircel.qml \
    qml/TopBarButton.qml \
    qml/main.qml \
    qml/Neuron.qml \
    qml/Net.qml \
    qml/MainInfo.qml \
    qml/SideBar.qml \
    qml/TopBar.qml \
    qml/CreatNewNet.qml

HEADERS += \
    mainwindow.h \
    $$netIncPath/net.h \
    $$netIncPath/neuron.h \
    $$netIncPath/backpropnet.h \
    $$netIncPath/geneticnet.h \
    $$netIncPath/savenet.h \
    netdatabase.h \
    thread.h \
    daten.h \
    datenset.h
