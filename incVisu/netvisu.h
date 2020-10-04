#ifndef NETVISU_H
#define NETVISU_H

#include <QObject>
//#include <backpropnet.h>
//#include <geneticnet.h>
#include <QThread>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QDebug>
#include <QQmlProperty>
#include <QQuickWidget>
#include "net.h"
#define sqlsave

#ifdef sqlsave
#include <savenetsql.h>
#endif

#define QmlRootContext "netListVisu"
#define QmlRootUiContext "netListUiVisu"
using namespace std;

class NetVisu : public QThread
{
    Q_OBJECT
public:
    explicit NetVisu(Net* _net,QObject *parent = nullptr);
    explicit NetVisu(vector<Net*> *_net,QObject *parent = nullptr);
    ~NetVisu();
    void setupNetVisu();
    void showWindow();
    void loadNetInUi(QQuickWidget* widget);

    vector<int> SourceNeuron;
    vector<int> DestinationNeuron;
    vector<int> NeuronTyp;
    vector<qreal> ConnectionWeight;
    vector<qreal> NeuronValueVect;
    QTimer *uTimer;
#ifdef sqlsave
    SaveNetSql db;
#endif
signals:
    void stopUpdateSignal();
    void startUpdateSignal();
    void setUpdateTimeSignal(unsigned int time);

    void updateVisu();
    void newValues();
    void updateNetStruc();
public slots:
    void updateValues();
    void setUpdateTime(unsigned int upDateTime);
    void displayUpdatNetTimer(const int &netId);
    int getHiddenX(const int &netId) ;
    int getHiddenY(const int &netId) ;
    int getInputs(const int &netId) ;
    int getOutputs(const int &netId) ;
    int getNetCount();

    qreal getNeuronValue(const int &netId , const int &neuronTypeId, const int type);
    int getNeuronID(const int &netId, const int &neuronTypeId, const int type);

    int getConns(const int &netId);
    int getConSourceID(const int &netId , const int &connId,  const int &connType=0) ;
    int getConDestinationID(const int &netId , const int &connId,  const int &connType=0) ;
    int getConSourceType(const int &netId , const int &connId,  const int &connType=0) ;
    int getConDestinationType(const int &netId , const int &connId,  const int &connType=0) ;

    qreal getConWeight(const int &netId , const int &connId) ;

    QVector<qreal> getRelX(const int &netId );//,, const int &neuronAbsId);
    QVector<qreal> getRelY(const int &netId );//, const int &neuronAbsId);

    QString getDBtPath();
    void setDBtPath(QString path);

    bool getBias(const int &netId) ;
    qreal getBiasValue(const int &netId) ;

    void stopUpdateSlot();
    void startUpdateSlot();

    void onRunDone(Net *p_net);
    void onBiasValueChanged(Net *p_net);
    void onWeightsChanged(Net *p_net);

    void onNetConfigurationChanged();
    void updateNetConfiguration();
    void saveRelPos(QVector<qreal> relX, QVector<qreal> relY,int netId);

    int addNewNet();
    void addInput(const int &netId);
    void addHiddenX(const int &netId);
    void addHiddenY(const int &netId);
    void addOutput(const int &netId);
private:
    bool access=false;

    QQmlApplicationEngine *engine;
    QQmlContext* context;
    vector<Net*> *netList;
    QQuickWidget* netWidget;


    vector<bool> disableNeuronSignalUpdateEvent;
    vector<bool> disableNetGenomUpdateEvent;
    vector<bool> disableNetBiasValueUpdateEvent;

    vector<vector<double>   > inputValueList;  //Index depending on NetID, secondDimension: depending on ID
    vector<vector<double>   > hiddenValueList; //Index depending on NetID, secondDimension: depending on neuron ID
    vector<vector<double>   > outputValueList; //Index depending on NetID, secondDimension: depending on the output
    vector<vector<double>   > genomList;       //Index depending on NetID, secondDimension: depending on the connection

    vector<double>  biasValueList; //Index depending on NetID

    unsigned getX(int netId ,int id);
    unsigned getY(int netId ,int id);
};

#endif // NETVISU_H
