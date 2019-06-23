#include <QtQuick/QQuickView>
#include <QGuiApplication>
#include <qqmlcontext.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<Mainwindow>("Mainwindowclass", 1, 0, "Ui");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();
    return app.exec();
}
