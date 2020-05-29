#-------------------------------------------------
#
# Project created by QtCreator 2020-02-06T10:11:04
#
#-------------------------------------------------

QT       += core gui qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = X-OR-GUI
TEMPLATE = app
QT_INSTALL_QML =
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

incPath = inc
srcPath = src

netIncPath = ../../inc
netSrcPath = ../../src
visuInc = ../../incVisu
visuSrc = ../../srcVisu
resourcePath = ../../res
INCLUDEPATH += $$netIncPath \
               $$visuInc \
               $$incPath


SOURCES += \
    $$srcPath/main.cpp \
    $$srcPath/mainwindow.cpp \
    $$netSrcPath/net.cpp \
    $$netSrcPath/neuron.cpp \
    $$netSrcPath/activation.cpp \
    $$netSrcPath/backpropnet.cpp \
    $$netSrcPath/savenet.cpp \
    $$netSrcPath/error.cpp \
    $$visuSrc/netvisu.cpp

HEADERS += \
    $$incPath/mainwindow.h \
    $$netIncPath/net.h \
    $$netIncPath/neuron.h \
    $$netIncPath/backpropnet.h \
    $$netIncPath/savenet.h \
    $$netIncPath/error.h \
    $$visuInc/netvisu.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target qml

RESOURCES += \
    $$resourcePath/qrc.qrc
