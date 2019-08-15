#ifndef THREAT_H
#define THREAT_H
#include <QThread>
#include <QObject>
#include <QString>
#include <backpropnet.h>
#include <QCoreApplication>
#include <iostream>
#include <daten.h>
using namespace std;

class netThread : public QThread
{
    Q_OBJECT
public:
    explicit netThread(QObject *parent = 0);
    void run();
    bool stop;
    float averageError();
    unsigned long learningSteps();
    unsigned int inputNeurons();
    unsigned int hiddenNeuronX();
    unsigned int hiddenNeuronY();
    unsigned int outputNeuron();
    void inputNeurons(unsigned int i);
    void hiddenNeuronX(unsigned int i);
    void hiddenNeuronY(unsigned int i);
    void outputNeuron(unsigned int i);
    float maxError();
    unsigned long maxSteps();
    void maxError(float i);
    void maxSteps(unsigned long i);
    void setupNet();
    bool bias();
    void bias(bool i);
    BackpropNet *net;
    Daten daten;
signals:
    void netTrained();
public slots:

private:
    unsigned int _animals;
    unsigned int _inputNeurons;
    unsigned int _hiddenNeuronX;
    unsigned int _hiddenNeuronY;
    unsigned int _outputNeuron;
    float _maxError;
    unsigned long _maxSteps;
    bool enableAverage;
    bool _bias;
    bool finished;

    FILE *genomlogFile;
    FILE *logfile;
    vector<float> genom;
    vector<float> outputVector;
    unsigned int _counter;
    unsigned int saveCounter;
    unsigned int saves;
    float _averageError;
    unsigned long _learningSteps;
    void logGenom(vector<float> genom);
};

#endif // THREAT_H
