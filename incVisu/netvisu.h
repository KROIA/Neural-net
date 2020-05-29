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
#include <QQuickWidget>

#define QmlRootContext "netListVisu"
#define QmlRootUiContext "netListUiVisu"
using namespace std;

class NetVisu : public QThread
{
    Q_OBJECT
public:
    explicit NetVisu(Net* _net,QObject *parent = nullptr);
    explicit NetVisu(vector<Net*> _net,QObject *parent = nullptr);
    void setupNetVisu();
    void showWindow();
    void loadNetInUi(QQuickWidget* widget);
    void setUpdateTime(QQuickWidget* widget,unsigned int upDateTime);
    vector<int> SourceNeuron;
    vector<int> DestinationNeuron;
    vector<int> NeuronTyp;
    vector<qreal> ConnectionWeight;
    vector<qreal> NeuronValueVect;

signals:
    void newNetData();
    void stopUpdateSignal();
    void startUpdateSignal();
public slots:

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

    void stopUpdateSlot();
    void startUpdateSlot();

    void onRunDone(Net *p_net);
    void onBiasValueChanged(Net *p_net);
    void onWeightsChanged(Net *p_net);

    void onNetConfigurationChanged();

private:
    bool access=false;
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    vector<Net*> netList;


    vector<vector<double>   > inputValueList;  //Index depending on NetID, secondDimension: depending on ID
    vector<vector<double>   > hiddenValueList; //Index depending on NetID, secondDimension: depending on neuron ID
    vector<vector<double>   > outputValueList; //Index depending on NetID, secondDimension: depending on the output
    vector<vector<double>   > genomList;       //Index depending on NetID, secondDimension: depending on the connection

    vector<double>  biasValueList; //Index depending on NetID
};

#endif // NETVISU_H
