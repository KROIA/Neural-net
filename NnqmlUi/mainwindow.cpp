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
    endNeuron();
}

Mainwindow::~Mainwindow()
{
    workThread->stop=false;
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

void Mainwindow::start(){
    workThread->stop =false;
    workThread->start();
    uiUpdateTimer->start(1000);
    qDebug()<<"stürzt nicht ab";
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
    //m_errorChart.push_back(qreal(workThread->averageError()));
    emit learningStepsChanged();
    emit averageErrorChanged();
    emit biasChanged();
    emit trainingSetChanged();
    emit outputSetChanged();
    emit errorChartChanged();
    emit netValueChanged();
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
    workThread->reset();
    workThread->start();
}
void Mainwindow::creatNew(float maxError, int maxSteps){
  stop();
  workThread->bias(bias());
  workThread->maxError(maxError);
  workThread->maxSteps(unsigned (long(maxSteps)));
  m_LayerId=0;
  m_NeuronId=0;
  m_errorChart.clear();
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
    vector<qreal> trainingSetVect;
    if(m_trainingSetId<int(workThread->daten.trainingInput.daten().size())){
    for (unsigned int i=0;i<workThread->daten.trainingInput.daten(0).size();++i) {
        trainingSetVect.push_back(qreal(workThread->daten.trainingInput.daten(unsigned(m_trainingSetId),i)));
    }
    }
    return trainingSetVect;
}

void Mainwindow::setTrainingSetId(const int &i){
    m_trainingSetId=i;
}

vector<qreal> Mainwindow::outputSet()const{
    vector<qreal> test;
    if(m_trainingSetId<int(workThread->daten.trainingOutput.daten().size())){
    for (unsigned i=0;i<workThread->daten.trainingOutput.daten(0).size();++i) {
        test.push_back(qreal(workThread->daten.trainingOutput.daten(unsigned(m_trainingSetId),i)));
    }
    }

    return test;
}
void Mainwindow::setOutputSet(const vector<qreal> &set){
    workThread->daten.trainingOutput.daten(set,unsigned(m_trainingSetId));
}
void Mainwindow::setTrainingSet(const vector<qreal> &set){
    workThread->daten.trainingInput.daten(set);
}
vector<qreal> Mainwindow::errorChart() const{
    return m_errorChart;
}
vector<qreal> Mainwindow::toQreal(vector<float> v){
    vector<qreal> returnVec;
    for (unsigned int i=0;i<v.size();++i) {
        returnVec.push_back(qreal(v[i]));
    }
    return  returnVec;
}
vector<float> Mainwindow::toFloat(vector<qreal> v){
    vector<float> returnVec;
    for (unsigned int i=0;i<v.size();++i) {
        returnVec.push_back(float(v[i]));
    }
    return returnVec;
}

vector<int> Mainwindow::startNeuron()const{
    vector<int> vect;
    vector<Connection> con=*workThread->net->connectionList();
    for (unsigned int i=0;i<workThread->net->connections();++i) {
        switch (con[i].source_ID.TYPE) {
            case 1:
                vect.push_back(int(con[i].source_ID.ID));
                break;
            case 5:
            //qDebug()<<"Bias";
                break;
            default:
                vect.push_back(int(workThread->net->inputNeurons()+con[i].source_ID.ID));
                break;
        }
    }
    return vect;
}
vector<int> Mainwindow::endNeuron()const{
    vector<int> vect;
    vector<Connection> con=*workThread->net->connectionList();
    for (unsigned int i=0;i<workThread->net->connections();++i) {
        switch (con[i].source_ID.TYPE) {
            case 5:
                break;
            default:
                vect.push_back(int(workThread->net->inputNeurons()+con[i].destination_ID.ID));
                break;
        }
    }
    return vect;
}


vector<qreal> Mainwindow::connectionWeight()const{
    vector<qreal> vect;

    vector<Connection> con=*workThread->net->connectionList();
    for (unsigned int i=0;i<workThread->net->connections();++i) {
        if(i<con.size()){
        if(con[i].source_ID.TYPE!=5&&con[i].destination_ID.TYPE!=5){
            vect.push_back(qreal(con[i].weight));
        }
        }
        else {
            qDebug()<<"add 1";
            vect.push_back(1);
        }
    }
    return vect;
}
vector<qreal> Mainwindow::neuronValueVect()const{
    vector<qreal> vect;
    vector<Neuron *> neuron=*workThread->net->allNeurons();
    for (unsigned int i=0;i<workThread->net->inputNeurons();++i) {
        vect.push_back(qreal(workThread->net->input(i)));
    }
    for (unsigned int i=0;i<workThread->net->allNeurons()->size();++i) {
        vect.push_back(qreal(neuron[i]->netInput()));
    }
    return vect;
}
vector<int> Mainwindow::neuronTyp()const{
    vector<int> vect;
    vector<Neuron *> neuron=*workThread->net->allNeurons();
    for (unsigned int i=0;i<workThread->net->inputNeurons();++i) {
        vect.push_back(1);
    }
    for (unsigned int i=0;i<workThread->net->allNeurons()->size();++i) {
        vect.push_back(neuron[i]->ID().TYPE);
    }
    return vect;
}
int Mainwindow::activFunc() const{
    return  workThread->net->activationFunction();
}

void Mainwindow::setActivFunc(const int &id){
    workThread->activFunc(id);
}

void Mainwindow::addNeuron(){

}

void Mainwindow::addConnection(unsigned int start, unsigned int end){
    workThread->net->connectNeuronViaID(start, end);
    emit netStructurChanged();
}
