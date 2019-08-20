#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include <QtQuick/QQuickView>
#include <QObject>
#include <QString>
#include <backpropnet.h>
#include <QCoreApplication>
#include <iostream>
#include <QColor>
#include "thread.h"
#include <netdatabase.h>
#include <QTimer>
using namespace std;

class Mainwindow: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int hiddenX READ hiddenX WRITE setHiddenX NOTIFY hiddenXChanged)
    Q_PROPERTY(int hiddenY READ hiddenY  WRITE setHiddenY NOTIFY hiddenYChanged)
    Q_PROPERTY(int output READ output WRITE setOutput NOTIFY outputChanged)
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)

    Q_PROPERTY(int connectionId WRITE setConnectionId)
    Q_PROPERTY(float connectionStrength READ connectionStrength NOTIFY connectionStrengthChanged)

    Q_PROPERTY(float neuronValue READ neuronValue NOTIFY neuronValueChanged)
    Q_PROPERTY(float averageError READ averageError NOTIFY averageErrorChanged)
    Q_PROPERTY(int learningSteps READ learningSteps NOTIFY learningStepsChanged)
    Q_PROPERTY(int maxLearningSteps READ maxLearningSteps WRITE setMaxLearningSteps NOTIFY maxLearningStepsChanged)
    Q_PROPERTY(int layerId WRITE setLayerId)
    Q_PROPERTY(int neuronId WRITE setNeuronId)
    //Q_PROPERTY(float biasValue READ biasValue)
    Q_PROPERTY(bool bias WRITE setBias READ bias NOTIFY biasChanged)
    Q_PROPERTY(std::vector<qreal> trainingSet READ trainingSet WRITE setTrainingSet NOTIFY trainingSetChanged)
    Q_PROPERTY(int trainingSetId WRITE setTrainingSetId)
    Q_PROPERTY(std::vector<qreal> outputSet READ outputSet WRITE setOutputSet NOTIFY outputSetChanged)
    Q_PROPERTY(std::vector<qreal> errorChart READ errorChart NOTIFY errorChartChanged)
public:
    explicit Mainwindow(QObject *parent = nullptr);
    ~Mainwindow();

    void setNeuronId(const int &id);
    void setConnectionId(const int &id);
    QString connectionColor() const;
    float connectionStrength() const;
    float biasValue() const;
    bool bias() const;
    int input() const;
    int hiddenX() const;
    int hiddenY() const;
    int output() const;
    void setBias(const bool &i);
    void setHiddenX(const int &i);
    void setHiddenY(const int &i);
    void setInput(const int &i);
    void setOutput(const int &i);
    void setTrainingSetId(const int &i);
    void setMaxLearningSteps(const int &i);
    float neuronValue() const;
    int learningSteps()const;
    int maxLearningSteps()const;
    float averageError() const;
    vector<qreal> trainingSet()const;
    vector<qreal> outputSet()const;
    vector<qreal> errorChart()const;
    void setOutputSet(const vector<qreal> &set);
    void setTrainingSet(const vector<qreal> &set);
    void setLayerId(const int &id);
    netThread *workThread;
    QTimer *uiUpdateTimer;
signals:
    void connectionStrengthChanged();
    void neuronValueChanged();
    void averageErrorChanged();
    void learningStepsChanged();
    void hiddenXChanged();
    void hiddenYChanged();
    void inputChanged();
    void outputChanged();
    void biasChanged();
    void trainingSetChanged();
    void maxLearningStepsChanged();
    void outputSetChanged();
    void errorChartChanged();
public slots:
    void start();
    void stop();
    void uiUpdate();
    void reset();
    void creatNew(float maxError, int maxSteps);
private:
    BackpropNet *net;
    NetDataBase db;
    int m_NeuronId;
    float m_NeuronValue;
    int m_ConnectionId;
    float m_connectionStrength;
    int m_LayerId;
    int m_trainingSetId;
    int m_outputSetId;
    vector<qreal> m_errorChart;
};

#endif // MAINWINDOW_H
