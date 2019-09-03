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
    Q_PROPERTY(float averageError READ averageError NOTIFY averageErrorChanged)
    Q_PROPERTY(int learningSteps READ learningSteps NOTIFY learningStepsChanged)
    Q_PROPERTY(int maxLearningSteps READ maxLearningSteps WRITE setMaxLearningSteps NOTIFY maxLearningStepsChanged)
    Q_PROPERTY(bool bias WRITE setBias READ bias NOTIFY biasChanged)
    Q_PROPERTY(std::vector<qreal> trainingSet READ trainingSet WRITE setTrainingSet NOTIFY trainingSetChanged)
    Q_PROPERTY(int trainingSetId WRITE setTrainingSetId)
    Q_PROPERTY(std::vector<qreal> outputSet READ outputSet WRITE setOutputSet NOTIFY outputSetChanged)
    Q_PROPERTY(std::vector<qreal> errorChart READ errorChart NOTIFY errorChartChanged)
    Q_PROPERTY(std::vector<int> startNeuron READ startNeuron NOTIFY netStructurChanged)
    Q_PROPERTY(std::vector<int> endNeuron READ endNeuron NOTIFY netStructurChanged)
    Q_PROPERTY(std::vector<int> neuronTyp READ neuronTyp NOTIFY netStructurChanged)
    Q_PROPERTY(std::vector<qreal> connectionWeight READ connectionWeight NOTIFY netValueChanged)
    Q_PROPERTY(std::vector<qreal> neuronValueVect READ neuronValueVect NOTIFY netValueChanged)
    Q_PROPERTY(int activFunc READ activFunc WRITE setActivFunc NOTIFY netStructurChanged)

public:
    explicit Mainwindow(QObject *parent = nullptr);
    ~Mainwindow();
    bool bias() const;
    int input() const;
    int hiddenX() const;
    int hiddenY() const;
    int output() const;
    int activFunc() const;
    void setActivFunc(const int &id);
    void setBias(const bool &i);
    void setHiddenX(const int &i);
    void setHiddenY(const int &i);
    void setInput(const int &i);
    void setOutput(const int &i);
    void setTrainingSetId(const int &i);
    void setMaxLearningSteps(const int &i);
    int learningSteps()const;
    int maxLearningSteps()const;
    float averageError() const;
    vector<qreal> trainingSet()const;
    vector<qreal> outputSet()const;
    vector<qreal> errorChart()const;
    vector<int> startNeuron()const;
    vector<int> endNeuron()const;
    vector<int> neuronTyp()const;
    vector<qreal> connectionWeight()const;
    vector<qreal> neuronValueVect()const;
    void setOutputSet(const vector<qreal> &set);
    void setTrainingSet(const vector<qreal> &set);
    netThread *workThread;
    QTimer *uiUpdateTimer;

signals:
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
    void netStructurChanged();
    void netValueChanged();

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
    vector<qreal> toQreal(vector<float> v);
    vector<float> toFloat(vector<qreal> v);
};

#endif // MAINWINDOW_H
