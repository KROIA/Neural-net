#ifndef NETVISU_H
#define NETVISU_H

#include <QObject>
#include <backpropnet.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QDebug>
#include <QQmlProperty>

using namespace std;

class NetVisu : public QObject
{
    Q_OBJECT
public:
    explicit NetVisu(BackpropNet *backnet,QObject *parent = nullptr);

    vector<int> SourceNeuron;
    vector<int> DestinationNeuron;
    vector<int> NeuronTyp;
    vector<qreal> ConnectionWeight;
    vector<qreal> NeuronValueVect;
signals:
    void newNetData();
public slots:
    void callMeFromQml();
    Q_INVOKABLE int getHiddenX() const;
    Q_INVOKABLE int getHiddenY() const;
    Q_INVOKABLE int getInputs() const;
    Q_INVOKABLE int getOutputs() const;
    Q_INVOKABLE QVector<qreal> getHiddenValue()const;
    Q_INVOKABLE QVector<qreal> getInputsValue() const;
    Q_INVOKABLE QVector<qreal> getOutputsValue() const;
    Q_INVOKABLE QVector<int> getHiddenID() const;
    Q_INVOKABLE QVector<int> getOutputID() const;

    Q_INVOKABLE QVector<int> getConSourceID() const;
    Q_INVOKABLE QVector<int> getConDestinationID() const;
    Q_INVOKABLE QVector<int> getConSourceType() const;
    Q_INVOKABLE QVector<int> getConDestinationType() const;
    Q_INVOKABLE QVector<qreal> getConWeight() const;
    Q_INVOKABLE bool getBias() const;
    Q_INVOKABLE qreal getBiasValue() const;

private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    int count;
    BackpropNet *net;
};

#endif // NETVISU_H
