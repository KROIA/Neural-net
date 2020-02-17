#include "netvisu.h"

NetVisu::NetVisu(BackpropNet *backnet,QObject *parent)
{
    engine = new QQmlApplicationEngine;
    context= new QQmlContext(engine);
    context=engine->rootContext();
    context->setContextProperty ("netVisu", this);
    engine->load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    count=0;
    net=new BackpropNet();
    net=backnet;
    StartNeuron=getStartNeuron();
    EndNeuron=getEndNeuron();
    NeuronTyp=getNeuronTyp();
    ConnectionWeight=getConnectionWeight();
    NeuronValueVect=getNeuronValueVect();
}

void NetVisu::callMeFromQml(){
    count++;
    emit increaseOne(QString::number(count));
}

int NetVisu::getHiddenX(){
    return int(net->get_hiddenNeuronsX());
}
int NetVisu::getHiddenY(){
    return int(net->get_hiddenNeuronsY());
}
int NetVisu::getInputs(){
    return int(net->get_inputNeurons());
}
int NetVisu::getOutputs(){
    return int(net->get_outputNeurons());
}
vector<int> NetVisu::getStartNeuron(){
    vector<int> vect;
    vector<Connection> con=net->get_connectionList();
    for (unsigned int i=0;i<con.size();++i) {
        switch (con[i].source_ID.TYPE) {
            case 1:
                vect.push_back(int(con[i].source_ID.ID));
                break;
            case 5:
            //qDebug()<<"Bias";
                break;
            default:
                vect.push_back(int(net->get_inputNeurons()+con[i].source_ID.ID));
                break;
        }
    }
    return vect;
}
vector<int> NetVisu::getEndNeuron(){
    vector<int> vect;
    vector<Connection> con=net->get_connectionList();
    for (unsigned int i=0;i<con.size();++i) {
        switch (con[i].source_ID.TYPE) {
            case 5:
                break;
            default:
                vect.push_back(int(net->get_inputNeurons()+con[i].destination_ID.ID));
                break;
        }
    }
    return vect;
}
vector<int> NetVisu::getNeuronTyp(){
    vector<int> vect;
    vector<Neuron*> neuron= *(net->get_ptr_allNeurons());
    for (unsigned int i=0;i<net->get_inputNeurons();++i) {
        vect.push_back(1);
    }
    for (unsigned int i=0;i<net->get_ptr_allNeurons()->size();++i) {
        vect.push_back((neuron[i]->get_ID().TYPE));
    }
    return vect;
}
vector<qreal> NetVisu::getConnectionWeight(){
    vector<qreal> vect;
    vector<Connection> con= net->get_connectionList();
    for (unsigned int i=0;i<con.size();++i) {
        if(i<con.size()){
        if(con[i].source_ID.TYPE!=5&&con[i].destination_ID.TYPE!=5){
            vect.push_back(qreal(con[i].weight));
        }
        }
        else {
            vect.push_back(1);
        }
    }
    return vect;
}
vector<qreal> NetVisu::getNeuronValueVect(){
    vector<qreal> vect;
    vector<Neuron*> *neuron= new vector<Neuron*>;
    neuron=net->get_ptr_allNeurons();
    for (unsigned int i=0;i<net->get_inputNeurons();++i) {
        vect.push_back(qreal(net->get_input(i)));
    }
    for (unsigned int i=0;i<net->get_ptr_allNeurons()->size();++i) {
        vect.push_back(qreal(net->get_input(i)));
    }
    return vect;
}
int NetVisu::getActivFunc(){
    return  net->get_activationFunction();
}
