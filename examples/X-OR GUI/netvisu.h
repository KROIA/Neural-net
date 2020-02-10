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
signals:
    void increaseOne(QString ms);
public slots:
    void callMeFromQml();
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    int count;
};

#endif // NETVISU_H
