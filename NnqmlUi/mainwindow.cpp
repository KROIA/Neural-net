#include "mainwindow.h"
Mainwindow::Mainwindow(QObject *parent) :
    QObject(parent)
{
    m_LayerId=0;
    m_NeuronId=0;
    workThread = new netThread(this);
    workThread->net->saveToNetFile("Test");
    uiUpdateTimer = new QTimer(this);
    connect(uiUpdateTimer, SIGNAL(timeout()),this,SLOT(uiUpdate()));
    uiUpdate();
    emit hiddenXChanged();
    emit hiddenYChanged();
    emit biasChanged();
}

Mainwindow::~Mainwindow()
{
    workThread->stop=false;
}

void Mainwindow::setNeuronId(const int &id){
    m_NeuronId=id;
    m_NeuronValue=2;
    int inputId=0;
    int outputId=int(workThread->net->hiddenNeuronsX())+1;
    if(inputId==m_LayerId&&unsigned(m_NeuronId)<workThread->net->inputNeurons()){
        m_NeuronValue=workThread->net->input(unsigned(m_NeuronId));
    }
    if(inputId<m_LayerId&&outputId>m_LayerId&&unsigned(m_NeuronId)<workThread->net->hiddenNeuronsY()){
        m_NeuronValue=workThread->net->hiddenNeuron((unsigned(m_LayerId)-1),unsigned(m_NeuronId))->output();
    }
    if(outputId==m_LayerId&&unsigned(m_NeuronId)<workThread->net->outputNeurons()){
        m_NeuronValue=workThread->net->outputNeuron(unsigned(m_NeuronId))->output();
    }
}

void Mainwindow::setConnectionId(const int &id){
    m_ConnectionId=id;
    int inputId=0;
    int outputId=int(workThread->net->hiddenNeuronsX())+1;
    if(inputId<m_LayerId&&outputId>m_LayerId&&unsigned(m_NeuronId)<workThread->net->hiddenNeuronsY()){
        m_connectionStrength=workThread->net->hiddenNeuron((unsigned(m_LayerId)-1),unsigned(m_NeuronId))->weight(unsigned(id));
    }
    if(outputId==m_LayerId&&unsigned(m_NeuronId)<workThread->net->outputNeurons()){
        m_connectionStrength=workThread->net->outputNeuron(unsigned(m_NeuronId))->weight(unsigned(id));
    }
}


float Mainwindow::connectionStrength() const{
    return m_connectionStrength;
}

int Mainwindow::input() const{
    return int(workThread->net->inputNeurons());
}
int Mainwindow::hiddenX() const{
    return int(workThread->net->hiddenNeuronsX());
}
int Mainwindow::hiddenY() const{
    return int(workThread->net->hiddenNeuronsY());
}
int Mainwindow::output() const{
    return int(workThread->net->outputNeurons());
}

void Mainwindow::setLayerId(const int &id){
    m_LayerId=id;
}

float Mainwindow::neuronValue() const{

    //qDebug()<<"return neuronValue: "<<m_NeuronValue;
    return m_NeuronValue;
}
void Mainwindow::start(){
    workThread->stop =false;
    workThread->start();
    uiUpdateTimer->start(1000);
}
void Mainwindow::setHiddenX(const int &i){
    workThread->hiddenNeuronX(unsigned(i));
}
void Mainwindow::setHiddenY(const int &i){
    workThread->hiddenNeuronY(unsigned(i));
}
void Mainwindow::setMaxLearningSteps(const int &i){
    workThread->maxSteps(unsigned(long(i)));
}
void Mainwindow::setInput(const int &i){
    workThread->inputNeurons(unsigned(i));
}
void Mainwindow::setOutput(const int &i){
    workThread->outputNeuron(unsigned(i));
}

bool Mainwindow::bias() const{
    return workThread->bias();
}
void Mainwindow::setBias(const bool &i){
    workThread->bias(i);
}
void Mainwindow::stop(){
    workThread->stop =true;
    uiUpdateTimer->stop();
}

void Mainwindow::uiUpdate(){
    net=workThread->net;
    emit neuronValueChanged();
    emit learningStepsChanged();
    emit averageErrorChanged();
    emit connectionStrengthChanged();
    emit biasChanged();
    emit trainingSetChanged();
    emit outputSetChanged();
}

int Mainwindow::learningSteps()const{
    return int(workThread->learningSteps());
}
int Mainwindow::maxLearningSteps()const{
    return int(workThread->maxSteps());
}
float Mainwindow::averageError() const{
    return float(workThread->averageError());
}
void Mainwindow::reset(){
    stop();
    workThread->setupNet();
    uiUpdate();
}
void Mainwindow::creatNew(float maxError, int maxSteps){
  stop();
  workThread->bias(bias());
  workThread->maxError(maxError);
  workThread->maxSteps(unsigned (long(maxSteps)));
  m_LayerId=0;
  m_NeuronId=0;
  reset();
  emit trainingSetChanged();
  emit outputSetChanged();
  emit hiddenXChanged();
  emit hiddenYChanged();
  emit biasChanged();
  emit outputChanged();
  emit inputChanged();
  qDebug()<<"creating finished";
}
vector<qreal> Mainwindow::trainingSet()const{
    vector<qreal> test;
    if(m_trainingSetId<int(workThread->trainingsSet.size())){
    for (int i=0;i<int(workThread->trainingsSet[0].size());++i) {
        test.push_back(qreal(workThread->trainingsSet[unsigned (long( long(m_trainingSetId)))][unsigned (long( long(i)))]));
    }
    }
    return test;
}

void Mainwindow::setTrainingSetId(const int &i){
    m_trainingSetId=i;
   //qDebug()<<i;
}

vector<qreal> Mainwindow::outputSet()const{
    vector<qreal> test;
    if(m_trainingSetId<int(workThread->outputSet.size())){
    for (int i=0;i<int(workThread->outputSet[0].size());++i) {
        test.push_back(qreal(workThread->outputSet[unsigned (long( long(m_trainingSetId)))][unsigned (long( long(i)))]));
    }
    }

    return test;
}
void Mainwindow::setOutputSet(const vector<qreal> &set){
    qDebug()<<"set output set:"<<set[m_trainingSetId];
    for (int i=0;i<int(set.size());++i) {
        workThread->outputSet[m_trainingSetId][0]=float(set[m_trainingSetId]);
}
    //qDebug()<<
}
void Mainwindow::setTrainingSet(const vector<qreal> &set){
    for (int i=0;i<int(set.size());++i) {
        qDebug()<<"set training set:"<<float(set[i]);
        workThread->trainingsSet[m_trainingSetId][i]=float(set[i]);
    }
}
