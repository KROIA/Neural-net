#ifndef NETVISU_H
#define NETVISU_H

#include <QObject>
//#include <backpropnet.h>
//#include <geneticnet.h>
#include <QThread>
#include <net.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QDebug>
#include <QQmlProperty>

#include <QGuiApplication>
using namespace std;

class NetVisu : public QThread
{
    Q_OBJECT
public:
    explicit NetVisu(Net* _net,QObject *parent = nullptr);
    explicit NetVisu(vector<Net*> _net,QObject *parent = nullptr);
    void setupQml();
    vector<int> SourceNeuron;
    vector<int> DestinationNeuron;
    vector<int> NeuronTyp;
    vector<qreal> ConnectionWeight;
    vector<qreal> NeuronValueVect;
signals:
    void newNetData();
public slots:
    void callMeFromQml();
    int getHiddenX(const int &netId) ;
    int getHiddenY(const int &netId) ;
    int getInputs(const int &netId) ;
    int getOutputs(const int &netId) ;
    int getNetCount();
    QVector<qreal> getHiddenValue(const int &netId);
    QVector<qreal> getInputsValue(const int &netId) ;
    QVector<qreal> getOutputsValue(const int &netId) ;
    QVector<int> getHiddenID(const int &netId) ;
    QVector<int> getOutputID(const int &netId) ;

    QVector<int> getConSourceID(const int &netId) ;
    QVector<int> getConDestinationID(const int &netId) ;
    QVector<int> getConSourceType(const int &netId) ;
    QVector<int> getConDestinationType(const int &netId) ;
    QVector<qreal> getConWeight(const int &netId) ;
    bool getBias(const int &netId) ;
    qreal getBiasValue(const int &netId) ;
    void run();
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    int count;
    vector<Net*> net;
};

#endif // NETVISU_H
