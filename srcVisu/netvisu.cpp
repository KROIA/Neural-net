#include "netvisu.h"
#include "neuron.h"

NetVisu::NetVisu(Net* _net,QObject *parent):
    QThread(parent)
{

    netList= new vector<Net*>;
    netList->push_back(_net);
    setupNetVisu();

    //db();
}

NetVisu::NetVisu(vector<Net*> *_net,QObject *parent):
    QThread(parent)
{

    netList=_net;
    setupNetVisu();
}

NetVisu::~NetVisu(){
    delete context;
    delete engine;
}

void NetVisu::setupNetVisu(){

    if(netList->size() == 0)
    {
        qDebug() << "Error: NetVisu::setupQml(): netList->size() == 0 | No Nets defined";
        return;
    }
    if((*netList)[0]->needsUpdate())
    {
        qDebug() << "Error: NetVisu::setupQml(): nets aren't updated, please call \"Net::updateNetConfiguration()\" before you create a Netvisu object";
        return;
    }

    netWidget = nullptr;

    disableNeuronSignalUpdateEvent  = vector<bool>(netList->size(),false);
    disableNetGenomUpdateEvent      = vector<bool>(netList->size(),false);
    disableNetBiasValueUpdateEvent  = vector<bool>(netList->size(),false);
    //Reserve the space for all nets
    inputValueList  = vector<vector<double>  >(netList->size(),vector<double>(0,0));
    hiddenValueList = vector<vector<double>  >(netList->size(),vector<double>(0,0));
    outputValueList = vector<vector<double>  >(netList->size(),vector<double>(0,0));
    genomList       = vector<vector<double>  >(netList->size(),vector<double>(0,0));
    biasValueList   = vector<double>          (netList->size(),0);

    for(unsigned i=0;i<netList->size();++i){
        connect((*netList)[i],SIGNAL(accessLock(Net *)),this,SLOT(stopUpdateSlot()));
        connect((*netList)[i],SIGNAL(accessUnlock(Net *)),this,SLOT(startUpdateSlot()));
        connect((*netList)[i],SIGNAL(netConfigurationUpdated(Net *)),this,SLOT(onNetConfigurationChanged()));
        connect((*netList)[i],SIGNAL(runDone(Net *)),this,SLOT(updateValues()));
    }


    vector<Net*> *saveVec=netList;
    /*for(int i=0;i<100;i++){
        saveVec.push_back(saveVec[0]);
    }*/
    db.saveNet((*saveVec));
    access=true;

}

void NetVisu::setUpdateTime(unsigned int upDateTime){
    emit setUpdateTimeSignal(upDateTime);
}

void NetVisu::showWindow(){
    engine = new QQmlApplicationEngine;
    context= new QQmlContext(engine);
    context=engine->rootContext();
    context->setContextProperty (QmlRootContext, this);
    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    //qDebug()<<"finished loading";
    startUpdateSlot();
    }

void NetVisu::loadNetInUi(QQuickWidget* widget){
    widget->rootContext()->setContextProperty(QmlRootContext,this);
    widget->setSource((QUrl(QStringLiteral("qrc:/qml/IntegratableNet/Main.qml"))));
    netWidget=widget;
}
void NetVisu::updateValues(){
    emit newValues();
}

void NetVisu::displayUpdatNetTimer(const int &netId){
    if(unsigned(netId) >= netList->size())
    {
        qDebug() << "Error: NetVisu::displayUpdatNetTimer(const int &netId ["<<netId<<"]) : netId out of range";
        return;
    }
    disableNeuronSignalUpdateEvent[unsigned(netId)]  = false;
    disableNetGenomUpdateEvent[unsigned(netId)]      = false;
    disableNetBiasValueUpdateEvent[unsigned(netId)]  = false;
}

int NetVisu::getHiddenX(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate()){
        qDebug()<<"Configuration needs update";
        return 0;}
    if(unsigned(netId)<netList->size()&&access){
        return int((*netList)[unsigned(netId)]->get_hiddenNeuronsX());
    }
    return 0;
}
int NetVisu::getHiddenY(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate()){
        qDebug()<<"Configuration needs update";
        return 0;}
    if(unsigned(netId)<netList->size()&&access){
        return int((*netList)[unsigned(netId)]->get_hiddenNeuronsY());
    }
    else{
        return 0;
    }
}
int NetVisu::getInputs(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate()){
        qDebug()<<"Configuration needs update";
        return 0;}
    if(unsigned(netId)<netList->size()&&access){
        return int((*netList)[unsigned(netId)]->get_inputNeurons());
    }
    else{

        return 0;
    }
}
int NetVisu::getOutputs(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate()){
        qDebug()<<"Configuration needs update";
        return 0;}
    if(unsigned(netId)<netList->size()&&access){
        return int((*netList)[unsigned(netId)]->get_outputNeurons());
    }
    else{
        if(!access)
            qDebug()<<"No Access to Net Configuration";
        return 0;
    }
}

qreal NetVisu::getNeuronValue(const int &netId , const int &neuronTypeId, const int type) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    NeuronType _type =NeuronType(type);
    if(_type==NeuronType::input){
        return qreal((*netList)[unsigned(netId)]->get_input(unsigned(neuronTypeId)));
    }
    if(_type==NeuronType::output){
        return qreal((*netList)[unsigned(netId)]->get_output(unsigned(neuronTypeId)));
    }
    if(_type==NeuronType::hidden){
        return qreal((*netList)[unsigned(netId)]->get_hidden(getX(netId,neuronTypeId),getY(netId,neuronTypeId)));
    }
    if(_type==NeuronType::bias){
        return qreal((*netList)[unsigned(netId)]->get_biasValue());
    }
    return 0;
}

int NetVisu::getNeuronID(const int &netId, const int &neuronTypeId, const int type) {
    //qDebug()<<"get vect "<<net->get_hiddenNeuronY_ptr(1).size();
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    NeuronType _type =NeuronType(type);
    if(_type==NeuronType::output){
        return int((*netList)[unsigned(netId)]->get_ptr_outputNeuron(unsigned(neuronTypeId))->get_ID().ID);
    }
    if(_type==NeuronType::hidden){
        return int((*netList)[unsigned(netId)]->get_ptr_hiddenNeuron(getX(netId,neuronTypeId),getY(netId,neuronTypeId))->get_ID().ID);
    }
    return 0;
}
int NetVisu::getConns(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    return int((*netList)[unsigned(netId)]->get_connections());
}

int NetVisu::getConSourceID(const int &netId, const int &connId,  const int &connType) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    if(unsigned(netId)<netList->size()&&access){
        if(connType==0){
            return int((*netList)[unsigned(netId)]->get_connectionList()[unsigned(connId)].source_ID.ID);
        }
        if(connType==1){
            return int((*netList)[unsigned(netId)]->get_costumConnectionList()[unsigned(connId)].source_ID.ID);
        }
    }
    return 0;
}

int NetVisu::getConDestinationID(const int &netId, const int &connId,  const int &connType) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    QVector<int> vect;
    if(unsigned(netId)<netList->size()&&access){
        if(connType==0){
            return int((*netList)[unsigned(netId)]->get_connectionList()[unsigned(connId)].destination_ID.ID);
        }
        if(connType==1){
            return int((*netList)[unsigned(netId)]->get_costumConnectionList()[unsigned(connId)].destination_ID.ID);
        }
    }
    return 0;

}

int NetVisu::getConSourceType(const int &netId, const int &connId,  const int &connType) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    if(unsigned(netId)<netList->size()&&access){
        if(connType==0){
            return int((*netList)[unsigned(netId)]->get_connectionList()[unsigned(connId)].source_ID.TYPE);
        }
        if(connType==1){
            return int((*netList)[unsigned(netId)]->get_costumConnectionList()[unsigned(connId)].source_ID.TYPE);
        }
    }
    return 0;
}

int NetVisu::getConDestinationType(const int &netId, const int &connId,  const int &connType) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    if(unsigned(netId)<netList->size()&&access){
        if(connType==0){
            return int((*netList)[unsigned(netId)]->get_connectionList()[unsigned(connId)].destination_ID.TYPE);
        }
        if(connType==1){
            return int((*netList)[unsigned(netId)]->get_costumConnectionList()[unsigned(connId)].destination_ID.TYPE);
        }
    }
    return 0;
}
qreal NetVisu::getConWeight(const int &netId, const int &connId) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    return (*netList)[unsigned(netId)]->get_genom()[unsigned(connId)];
}
QString NetVisu::getDBtPath(){
    return db.getDatabaseName();
}
void NetVisu::setDBtPath(QString path){
    db.setDatabaseName(path);
}
QVector<qreal> NetVisu::getRelX(const int &netId){
    return db.loadRelXPos((*netList)[unsigned(netId)]);
    //return db.loadRelXPos((*netList)[unsigned(netId)])[neuronAbsId];
}

QVector<qreal> NetVisu::getRelY(const int &netId){
    return db.loadRelYPos((*netList)[unsigned(netId)]);
    //return db.loadRelYPos((*netList)[unsigned(netId)])[neuronAbsId];
}

bool NetVisu::getBias(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return false;
    if(unsigned(netId)<netList->size()&&access){
    return (*netList)[unsigned(netId)]->get_bias();
    }
    else{
        return false;
    }
}

qreal NetVisu::getBiasValue(const int &netId) {
    if((*netList)[unsigned(netId)]->needsUpdate())
        return 0;
    if(unsigned(netId)<netList->size()&&access){
    return (*netList)[unsigned(netId)]->get_biasValue();
    }
    else{
        return 0;
    }
}

int NetVisu::getNetCount(){
    return int(netList->size());
}
void NetVisu::stopUpdateSlot(){
    access=false;
    emit stopUpdateSignal();
}
void NetVisu::startUpdateSlot(){
    access=true;
    emit startUpdateSignal();
}

void NetVisu::onRunDone(Net *p_net)
{
    if(!access || disableNeuronSignalUpdateEvent[p_net->get_ID()])
        return;
    unsigned int netID              = p_net->get_ID();
    inputValueList[netID]           = p_net->get_input();
    hiddenValueList[netID]          = p_net->get_hidden();
    outputValueList[netID]          = p_net->get_output();
    disableNeuronSignalUpdateEvent[p_net->get_ID()]  = true;
}

void NetVisu::onBiasValueChanged(Net *p_net)
{
    if(!access || disableNetBiasValueUpdateEvent[p_net->get_ID()])
        return;
    unsigned int netID              = p_net->get_ID();
    biasValueList[netID]            = p_net->get_biasValue();
    disableNetBiasValueUpdateEvent[p_net->get_ID()]  = true;
}
void NetVisu::onWeightsChanged(Net *p_net)
{
    if(!access || disableNetGenomUpdateEvent[p_net->get_ID()])
        return;
    unsigned int netID          = p_net->get_ID();
    genomList[netID]            = p_net->get_genom();
    disableNetGenomUpdateEvent[p_net->get_ID()]  = true;
}
void NetVisu::onNetConfigurationChanged()
{
    for(unsigned i=0;i<netList->size();++i){
        connect((*netList)[i],SIGNAL(accessUnlock()),this,SLOT(updateNetConfiguration()));
    }
}

void NetVisu::updateNetConfiguration(){
    for(unsigned i=0;i<netList->size();++i){
        disconnect((*netList)[i],SIGNAL(accessUnlock()),this,SLOT(updateNetConfiguration()));
    }
    emit updateNetStruc();
}

void NetVisu::saveRelPos(QVector<qreal> relX, QVector<qreal> relY,int netId){
    if(netId<int(netList->size())){
        db.saveRelPos(relX,relY,(*netList)[unsigned(netId)]);
    }
}

int NetVisu::addNewNet(){
    qDebug()<<"new Net Id";
    Net* newNet;
    newNet= new Net();
    newNet->set_hiddenNeuronsX(0);
    newNet->set_hiddenNeuronsY(0);
    newNet->set_bias(false);
    newNet->set_inputNeurons(1);
    newNet->updateNetConfiguration();
    if(newNet->get_ptr_allNeurons()->size()>1){
    }

    newNet->updateNetConfiguration();
    netList->push_back(newNet);
    connect((*netList)[netList->size()-1],SIGNAL(accessLock()),this,SLOT(stopUpdateSlot()));
    connect((*netList)[netList->size()-1],SIGNAL(accessUnlock()),this,SLOT(startUpdateSlot()));
    emit updateNetStruc();
    emit updateVisu();
    return int(netList->size()-1);
}

void NetVisu::addInput(const int &netId){
    (*netList)[unsigned(netId)]->set_inputNeurons((*netList)[unsigned(netId)]->get_inputNeurons()+1);
    (*netList)[unsigned(netId)]->updateNetConfiguration();
    emit updateNetStruc();
}
void NetVisu::addHiddenX(const int &netId){
    (*netList)[unsigned(netId)]->set_hiddenNeuronsX((*netList)[unsigned(netId)]->get_hiddenNeuronsX()+1);
    (*netList)[unsigned(netId)]->updateNetConfiguration();
    emit updateNetStruc();
}
void NetVisu::addHiddenY(const int &netId){
    (*netList)[unsigned(netId)]->set_hiddenNeuronsY((*netList)[unsigned(netId)]->get_hiddenNeuronsY()+1);
    (*netList)[unsigned(netId)]->updateNetConfiguration();
    emit updateNetStruc();
}
void NetVisu::addOutput(const int &netId){
    (*netList)[unsigned(netId)]->set_outputNeurons((*netList)[unsigned(netId)]->get_outputNeurons()+1);
    (*netList)[unsigned(netId)]->updateNetConfiguration();
    emit updateNetStruc();
}

unsigned NetVisu::getX(int netId ,int id){
    return unsigned(id-int(getY(netId,id)))/(*netList)[unsigned(netId)]->get_hiddenNeuronsY();
}
unsigned NetVisu::getY(int netId ,int id){
    return unsigned(id%int((*netList)[unsigned(netId)]->get_hiddenNeuronsY()));
}
