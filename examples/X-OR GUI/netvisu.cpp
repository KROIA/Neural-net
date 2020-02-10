#include "netvisu.h"

NetVisu::NetVisu(BackpropNet *backnet,QObject *parent)
{
    engine = new QQmlApplicationEngine;
    context= new QQmlContext(engine);
    context=engine->rootContext();
    context->setContextProperty ("netVisu", this);
    engine->load(QUrl(QStringLiteral("qrc:/Main.qml")));
    count=0;
}

void NetVisu::callMeFromQml(){
    count++;
    emit increaseOne(QString::number(count));
}
