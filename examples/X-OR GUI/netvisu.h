#ifndef NETVISU_H
#define NETVISU_H

#include <QObject>
#include <backpropnet.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QDebug>
#include <QQmlProperty>

class NetVisu : public QObject
{
    Q_OBJECT
public:
    explicit NetVisu(BackpropNet *backnet,QObject *parent = nullptr);
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
};

#endif // NETVISU_H
