#include "netvisu.h"
#include "neuron.h"
NetVisu::NetVisu(BackpropNet *backnet,QObject *parent)
{
    engine = new QQmlApplicationEngine;
    context= new QQmlContext(engine);
    context=engine->rootContext();
    context->setContextProperty ("netVisu", this);
    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    count=0;
    net=new BackpropNet();
    net=backnet;
}

void NetVisu::callMeFromQml(){
    emit newNetData();
}

int NetVisu::getHiddenX() const{
    return int(net->get_hiddenNeuronsX());
}
int NetVisu::getHiddenY() const{
    return int(net->get_hiddenNeuronsY());
}
int NetVisu::getInputs() const{

    return int(net->get_inputNeurons());
}
int NetVisu::getOutputs() const{
    return int(net->get_outputNeurons());
}

QVector<qreal> NetVisu::getHiddenValue() const{
    QVector<qreal> vect;
    for(unsigned int j=0;j<net->get_hiddenY(0).size();j++){
        vect.push_back(qreal(net->get_hiddenY(0)[j]));
    }
    return vect;
}
QVector<qreal> NetVisu::getInputsValue() const {
    return QVector<qreal>::fromStdVector(net->get_input());
}
QVector<qreal> NetVisu::getOutputsValue() const{
    return QVector<qreal>::fromStdVector(net->get_output());
}

QVector<int> NetVisu::getHiddenID() const{
    qDebug()<<"get vect "<<net->get_hiddenNeuronY_ptr(1).size();
    QVector<int> vect;
        for(unsigned int j=0;j<net->get_hiddenNeuronY_ptr(0).size();j++){
            vect.push_back(j);
            //vect.push_back(int(net->get_hiddenNeuronY_ptr(0)[j]->get_ID().ID));
            qDebug()<<j;
        }
        qDebug()<<vect;
    return vect;
}
QVector<int> NetVisu::getOutputID() const{
    QVector<int> vect;
        for(unsigned int j=0;j<net->get_outputNeurons();j++){
            vect.push_back(net->get_ptr_outputNeuron(j)->get_ID().ID);
        }
    return vect;
}

QVector<int> NetVisu::getConSourceID() const{
    QVector<int> vect;
    for(unsigned int i=0;i<net->get_connections();i++){
        vect.push_back(net->get_connectionList()[i].source_ID.ID);
    }
    return vect;
}

QVector<int> NetVisu::getConDestinationID() const{
    QVector<int> vect;
    for(unsigned int i=0;i<net->get_connections();i++){
        vect.push_back(net->get_connectionList()[i].destination_ID.ID);
    }
    return vect;
}

QVector<int> NetVisu::getConSourceType() const{
    QVector<int> vect;
    for(unsigned int i=0;i<net->get_connections();i++){
        vect.push_back(net->get_connectionList()[i].source_ID.TYPE);
    }
    return vect;
}

QVector<int> NetVisu::getConDestinationType() const{
    QVector<int> vect;
    for(unsigned int i=0;i<net->get_connections();i++){
        vect.push_back(net->get_connectionList()[i].destination_ID.TYPE);
    }
    return vect;
}
QVector<qreal> NetVisu::getConWeight() const{
    QVector<qreal> vect;
    for(unsigned int i=0;i<net->get_connections();i++){
        vect.push_back(net->get_connectionList()[i].weight);
    }
    return vect;
}


bool NetVisu::getBias() const{
    return net->get_bias();
}

qreal NetVisu::getBiasValue() const{
    return net->get_biasValue();
}
