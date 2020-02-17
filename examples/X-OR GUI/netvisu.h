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

    vector<int> StartNeuron;
    vector<int> EndNeuron;
    vector<int> NeuronTyp;
    vector<qreal> ConnectionWeight;
    vector<qreal> NeuronValueVect;
signals:
    void increaseOne(QString ms);
public slots:
    void callMeFromQml();
    int getHiddenX();
    int getHiddenY();
    int getInputs();
    int getOutputs();


    vector<int> getStartNeuron();
    vector<int> getEndNeuron();
    vector<int> getNeuronTyp();
    vector<qreal> getConnectionWeight();
    vector<qreal> getNeuronValueVect();
    int getActivFunc();
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    int count;
    BackpropNet *net;
};

#endif // NETVISU_H
