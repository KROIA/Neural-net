#-------------------------------------------------
#
# Project created by QtCreator 2019-05-27T17:10:35
#
#-------------------------------------------------

QT       += core gui charts qml quick quickwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app

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
DLL_buildPath = bin/64bit/build-Release

#-------------------------------------------------------
#---This Project needs external code--------------------
# Qect.dll : https://github.com/KROIA/Rect
# geometry : https://github.com/KROIA/Geometry

#change your PC-username
#username     = SCHEH
#QT_work_dir  = C:/Users/SCHEH/Documents/GitHub/
#username     = AlexKrieg
#QT_work_dir  = C:/Users/$$username/Documents/QT

username     = Hannes
QT_work_dir  = C:/Users/Hannes/Documents/Programme/GitHub
geometryPath = $$QT_work_dir/lib/Geometry
rectPath     = $$QT_work_dir/DLL/Rect
configPath   = $$QT_work_dir/DLL/Config

#geometryPath = $$QT_work_dir/lib/Geometry
#rectPath     = $$QT_work_dir/DLL/Rect
#configPath   = $$QT_work_dir/DLL/Config



#username     = AlexKrieg
#QT_work_dir  = C:/Users/$$username/Documents/QT

geometryPath = $$QT_work_dir/lib/Geometry
rectPath     = $$QT_work_dir/DLL/Rect
configPath   = $$QT_work_dir/DLL/Config

LIBS+=$$rectPath/$$DLL_buildPath/Rect.dll \
      $$configPath/$$DLL_buildPath/Config.dll

incPath = inc
srcPath = src

netIncPath = ../../inc
netSrcPath = ../../src
visuInc = ../../incVisu
visuSrc = ../../srcVisu
resourcePath = ../../res

INCLUDEPATH += $$netIncPath \
               $$incPath \
               $$rectPath \
               $$geometryPath \
               $$configPath \
               $$visuInc


SOURCES += \
    $$srcPath/main.cpp \
    $$srcPath/snake.cpp  \
    $$netSrcPath/net.cpp \
    $$netSrcPath/neuron.cpp \
    $$netSrcPath/activation.cpp \
    $$netSrcPath/geneticnet.cpp \
    $$netSrcPath/backpropnet.cpp \
    $$netSrcPath/savenet.cpp \
    $$netSrcPath/error.cpp \
    $$geometryPath/geometry.cpp \
    $$srcPath/player.cpp \
    $$srcPath/food.cpp \
    $$srcPath/maptile.cpp \
    $$srcPath/environment.cpp \
    $$visuSrc/netvisu.cpp

HEADERS += \
    $$incPath/snake.h \
    $$geometryPath/geometry.h \
    $$incPath/player.h \
    $$incPath/food.h \
    $$incPath/maptile.h \
    $$incPath/environment.h \
    $$netIncPath/net.h \
    $$netIncPath/neuron.h \
    #$$netIncPath/activation.h \
    $$netIncPath/savenet.h \
    $$netIncPath/error.h \
    $$netIncPath/geneticnet.h \
    $$netIncPath/backpropnet.h \
    $$visuInc/netvisu.h

FORMS += \
        snake.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target qml

RESOURCES += \
    $$resourcePath/qrc.qrc
