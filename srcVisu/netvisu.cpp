#include "netvisu.h"
#include "neuron.h"

NetVisu::NetVisu(Net* _net,QObject *parent):
    QThread(parent)
{
    net.push_back(_net);
    setupQml();
}

NetVisu::NetVisu(vector<Net*> _net,QObject *parent):
    QThread(parent)
{
    net=_net;
    setupQml();
}

void NetVisu::setupQml(){
    for(unsigned i=0;i<net.size();++i){
        connect(net[i],SIGNAL(accessLock()),this,SLOT(stopUpdateSlot()));
        connect(net[i],SIGNAL(accessUnlock()),this,SLOT(startUpdateSlot()));
    }
    engine = new QQmlApplicationEngine;
    context= new QQmlContext(engine);
    context=engine->rootContext();
    context->setContextProperty ("netVisu", this);
    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    startUpdateSlot();
}

void NetVisu::callMeFromQml(){
    emit newNetData();
}

int NetVisu::getHiddenX(const int &netId) {
    if(unsigned(netId)<net.size()&&access){
        return int(net[unsigned(netId)]->get_hiddenNeuronsX());
    }
    return 0;
}
int NetVisu::getHiddenY(const int &netId) {
    if(unsigned(netId)<net.size()&&access){
        return int(net[unsigned(netId)]->get_hiddenNeuronsY());
    }
    else{
        return 0;
    }
}
int NetVisu::getInputs(const int &netId) {
    if(unsigned(netId)<net.size()&&access){
        return int(net[unsigned(netId)]->get_inputNeurons());
    }
    else{
        return 0;
    }
}
int NetVisu::getOutputs(const int &netId) {
    if(unsigned(netId)<net.size()&&access){
        return int(net[unsigned(netId)]->get_outputNeurons());
    }
    else{
        return 0;
    }
}

QVector<qreal> NetVisu::getHiddenValue(const int &netId) {
    QVector<qreal> vect;
    if(unsigned(netId)<net.size()&&access){
        std::vector<Neuron*> hiddenNeuronList = *net[unsigned(netId)]->get_ptr_allNeurons();
        for(unsigned  int j=0;j<hiddenNeuronList.size();j++){
            if(hiddenNeuronList[j]->get_ID().TYPE == NeuronType::hidden)
            {
                vect.push_back(hiddenNeuronList[j]->get_output());
            }
            //vect.push_back(int(net->get_hiddenNeuronY_ptr(0)[j]->get_ID().ID));
        }
    }
    return vect;
}
QVector<qreal> NetVisu::getInputsValue(const int &netId) {
    QVector<qreal> vect;
    if(unsigned(netId)<net.size()&&access){
        vect =QVector<qreal>::fromStdVector(net[unsigned(netId)]->get_input());
    }
    return vect;
}
QVector<qreal> NetVisu::getOutputsValue(const int &netId) {
    QVector<qreal> vect;
    if(unsigned(netId)<net.size()&&access){
        vect =QVector<qreal>::fromStdVector(net[unsigned(netId)]->get_output());
    }
    return vect;
}

QVector<int> NetVisu::getHiddenID(const int &netId) {
    //qDebug()<<"get vect "<<net->get_hiddenNeuronY_ptr(1).size();
    QVector<int> vect;
    if(unsigned(netId)<net.size()&&access){
        std::vector<Neuron*> hiddenNeuronList = *net[unsigned(netId)]->get_ptr_allNeurons();
        for(unsigned  int j=0;j<hiddenNeuronList.size();j++){
            if(hiddenNeuronList[j]->get_ID().TYPE == NeuronType::hidden)
            {
                vect.push_back(hiddenNeuronList[j]->get_ID().ID);
            }
            //vect.push_back(int(net->get_hiddenNeuronY_ptr(0)[j]->get_ID().ID));
        }
    }
    return vect;
}
QVector<int> NetVisu::getOutputID(const int &netId) {
    QVector<int> vect;
    if(unsigned(netId)<net.size()&&access){
        for(unsigned  int j=0;j<net[unsigned(netId)]->get_outputNeurons();j++){
            vect.push_back(net[unsigned(netId)]->get_ptr_outputNeuron(j)->get_ID().ID);
        }
    }
    return vect;
}

QVector<int> NetVisu::getConSourceID(const int &netId) {
    QVector<int> vect;
    if(unsigned(netId)<net.size()&&access){
        for(unsigned  int i=0;i<net[unsigned(netId)]->get_connections();i++){
            vect.push_back(net[unsigned(netId)]->get_connectionList()[i].source_ID.ID);
        }
    }
    //qDebug()<<"Source \t"<<vect;
    return vect;
}

QVector<int> NetVisu::getConDestinationID(const int &netId) {
    QVector<int> vect;
    if(unsigned(netId)<net.size()&&access){
        for(unsigned  int i=0;i<net[unsigned(netId)]->get_connections();i++){
            vect.push_back(net[unsigned(netId)]->get_connectionList()[i].destination_ID.ID);
        }
    }
    //qDebug()<<"\t"<<vect;
    return vect;
}

QVector<int> NetVisu::getConSourceType(const int &netId) {
    QVector<int> vect;
    if(unsigned(netId)<net.size()&&access){
        for(unsigned  int i=0;i<net[unsigned(netId)]->get_connections();i++){
            vect.push_back(net[unsigned(netId)]->get_connectionList()[i].source_ID.TYPE);
        }
    }
    return vect;
}

QVector<int> NetVisu::getConDestinationType(const int &netId) {
    QVector<int> vect;
    if(unsigned(netId)<net.size()&&access){
        for(unsigned  int i=0;i<net[unsigned(netId)]->get_connections();i++){
            vect.push_back(net[unsigned(netId)]->get_connectionList()[i].destination_ID.TYPE);
        }
    }
    return vect;
}
QVector<qreal> NetVisu::getConWeight(const int &netId) {
    QVector<qreal> vect;
    if(unsigned(netId)<net.size()&&access){
        vect= QVector<qreal>::fromStdVector(net[unsigned(netId)]->get_genom());
    }
    return vect;
}


bool NetVisu::getBias(const int &netId) {
    if(unsigned(netId)<net.size()&&access){
    return net[unsigned(netId)]->get_bias();
    }
    else{
        return false;
    }
}

qreal NetVisu::getBiasValue(const int &netId) {
    if(unsigned(netId)<net.size()&&access){
    return net[unsigned(netId)]->get_biasValue();
    }
    else{
        return 0;
    }
}

int NetVisu::getNetCount(){
    return net.size();
}
void NetVisu::stopUpdateSlot(){
    qDebug()<<"stop Update";
    access=false;
    emit stopUpdateSignal();
}
void NetVisu::startUpdateSlot(){
    qDebug()<<"start Update";
    access=true;
    emit startUpdateSignal();
}

