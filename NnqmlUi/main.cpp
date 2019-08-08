#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<Mainwindow>("Mainwindowclass", 1, 0, "Ui");
    //QQuickView view;
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    //view.show();
    return app.exec();
}
