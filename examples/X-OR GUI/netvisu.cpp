#include "netvisu.h"

NetVisu::NetVisu(BackpropNet *backnet,QObject *parent)
{
    engine = new QQmlApplicationEngine;
    context= new QQmlContext(engine);
    context=engine->rootContext();
    context->setContextProperty ("dataStore", this);
    engine->load(QUrl(QStringLiteral("qrc:/Main.qml")));
}
