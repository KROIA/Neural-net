#include "netdatabase.h"

NetDataBase::NetDataBase()
{
    _savepath.setUrl("C:/Users/hsche/Desktop/");
    _dbfilename="savedb.db";
    netId=0;

    creatTables();

}
void NetDataBase::command(QString command){
    commandOutput(command);
    closeDb();
}
void NetDataBase::commandOutput(QString command){
    openDb();
    query = new QSqlQuery(mydb);

    query->prepare(command);
    bool fail;
    fail=query->exec();
    /*if(fail==false){
        qDebug()<<"SQL Command failed:";
        qDebug()<<command;
        qDebug()<<"";
    }*/
    query->first();
}
void NetDataBase::openDb(){
    mydb = QSqlDatabase::addDatabase("QSQLITE","connction1");
    //mydb = QSqlDatabase::database("connction1");
    mydb.setDatabaseName(_savepath.toString()+_dbfilename);
    if (!mydb.open())
          qDebug()<<"DB is not connected.";
}
void NetDataBase::closeDb(){
    QString connection;
    connection = mydb.connectionName();
    mydb.close();
    mydb = QSqlDatabase();
    mydb.removeDatabase(connection);
}

void NetDataBase::creatTables(){
command("CREATE TABLE BackPropNet( "
        "BackPropNet_id integer PRIMARY KEY, "
        "MutationFactor real NOT NULL,"
        "Net_id integer NOT NULL,"
        "FOREIGN KEY (net_id) REFERENCES Net(Net_id)"
        "ON DELETE CASCADE);"
        );
command("CREATE TABLE Net( "
        "Net_id integer PRIMARY KEY, "
        "InputNeurons integer NOT NULL,"
        "OutputNeurons integer NOT NULL,"
        "HiddenNeuronsX integer NOT NULL,"
        "HiddenNeuronsY integer NOT NULL,"
        "Bias bool,"
        "BiasValue real,"
        "EnableAverage bool,"
        "ActivationFunction "
        "INTEGER NOT NULL);"
        );
command("CREATE TABLE Neuron( "
        "Neuron_id integer PRIMARY KEY, "
        "YPosition integer NOT NULL,"
        "XPosition integer NOT NULL,"
        "Net_id integer NOT NULL,"
        "FOREIGN KEY (Net_id) REFERENCES Net(Net_id)"
        "ON DELETE CASCADE);"
        );
command("CREATE TABLE Connection( "
        "Connection_id integer PRIMARY KEY, "
        "Neuron integer NOT NULL,"
        "Weight real NOT NULL,"
        "Weight_id integer NOT NULL, "
        "Net_id integer NOT NULL,"
        //"FOREIGN KEY (StartNeuron,EndNeuron) REFERENCES Neuron(Neuron_id,Neuron_id) "
        "FOREIGN KEY (Net_id) REFERENCES Net(Net_id)"
        "ON DELETE CASCADE);"
        );
}

void NetDataBase::saveBackpropNet(BackpropNet *backPropNet){
    deleteBackpropNet(netId);
    saveNet(backPropNet);
    command("INSERT INTO BackPropNet "
            "VALUES("+QString::number(netId)+", "
            +QString::number(double(backPropNet->mutationFactor()))+", "
            +QString::number(netId)+");");
}

void NetDataBase::saveNet(Net *net){
    command("INSERT INTO Net "
            "VALUES("+QString::number(netId)+", "
            +QString::number(net->inputNeurons())+", "
            +QString::number(net->outputNeurons())+","
            +QString::number(net->hiddenNeuronsX())+", "
            +QString::number(net->hiddenNeuronsY())+", "
            +QString::number(net->bias())+","
            +QString::number(double(net->biasValue()))+","
            +QString::number(net->enableAverage())+","
            +QString::number(net->activationFunction())+")");
    saveNeuron(net);
    for (unsigned int i=0;i<net->hiddenNeuronsX()+1;++i) {
       if(i==net->hiddenNeuronsX()){
            for (unsigned int j=0;j<net->outputNeurons();++j) {
                commandOutput("SELECT Neuron_id FROM Neuron WHERE XPosition="+QString::number(i)+" AND YPosition="+QString::number(j)+"");
                unsigned int id=query->value(0).toUInt();
                closeDb();
                saveConnection(net->outputNeuron(j), id);
            }
        }
        if(i<net->hiddenNeuronsX()){
            for (unsigned int j=0;j<net->hiddenNeuronsY();++j) {
                commandOutput("SELECT Neuron_id FROM Neuron WHERE XPosition="+QString::number(i)+" AND YPosition="+QString::number(j)+"");
                unsigned int id=query->value(0).toUInt();
                closeDb();
                saveConnection(net->hiddenNeuron(i,j),id);
            }
        }
    }
}
void NetDataBase::saveNeuron(Net *net){
    for (unsigned int i=0;i<=net->hiddenNeuronsX();++i) {
       if(i==net->hiddenNeuronsX()){
            for (unsigned int j=0;j<net->outputNeurons();++j) {
                command("INSERT INTO Neuron(XPosition, YPosition, Net_id) "
                        "VALUES("+QString::number(i)+", "
                        +QString::number(j)+", "+QString::number(netId)+")");
            }
        }
        if(i<net->hiddenNeuronsX()){
            for (unsigned int j=0;j<net->hiddenNeuronsY();++j) {
                command("INSERT INTO Neuron(XPosition, YPosition, Net_id) "
                        "VALUES("+QString::number(i)+", "
                        +QString::number(j)+", "+QString::number(netId)+")");
            }
        }
    }


}
void NetDataBase::saveConnection(Neuron *neuron, unsigned int neuronId){
    for (unsigned int i=0;i<neuron->weight().size();++i) {
        command("INSERT INTO Connection(Neuron,Weight,Weight_id,Net_id) "
                "VALUES("+QString::number(neuronId)+", "
                ""+QString::number(double(neuron->weight(i)))+","
                +QString::number(i)+", "
                +QString::number(netId)+")");
    }
}

void NetDataBase::deleteBackpropNet(unsigned int id){
    command("DELETE FROM Net WHERE "+QString::number(id)+"= Net_id");
    command("DELETE FROM BackPropNet WHERE "+QString::number(id)+"= Net_id");
    command("DELETE FROM Connection WHERE "+QString::number(id)+"= Net_id");
    command("DELETE FROM Neuron WHERE "+QString::number(id)+"= Net_id");
}
///////////////////////////////////LoadNet///////////////////////////////////////////////////////////////
void NetDataBase::loadBackPropNet(BackpropNet *net){
    unsigned int id=netId;
    loadNet(id, net);
    commandOutput("SELECT MutationFactor FROM BackPropNet WHERE "+QString::number(id)+"=Net_id");
    net->mutationFactor(query->value(0).toFloat());
    closeDb();
    loadNet(id, net);
}
void NetDataBase::loadNet(unsigned int id, Net *net){
    commandOutput("SELECT InputNeurons, OutputNeurons, HiddenNeuronsX,"
                  "HiddenNeuronsY,Bias,BiasValue,EnableAverage,ActivationFunction FROM Net "
                  "WHERE Net_id="+QString::number(id));
    net->inputNeurons(query->value(0).toUInt());
    net->outputNeurons(query->value(1).toUInt());
    net->hiddenNeuronsX(query->value(2).toUInt());
    net->hiddenNeuronsY(query->value(3).toUInt());
    net->bias(query->value(4).toBool());
    net->biasValue(query->value(5).toFloat());
    net->activationFunction(Sigmoid);
    closeDb();
    loadNeuron(net);
}

void NetDataBase::loadNeuron(Net *net){
    commandOutput("SELECT Neuron.XPosition,Neuron.YPosition,Connection.Weight,Connection.Weight_id "
                  "FROM Neuron,Connection WHERE Neuron.Neuron_id=Connection.Neuron");
    do{
        if(query->value(0).toUInt()<net->hiddenNeuronsX()){
            net->hiddenNeuron(query->value(0).toUInt(),query->value(1).toUInt())->weight(query->value(3).toUInt(),query->value(2).toFloat());
        }
        else {
            net->outputNeuron(query->value(1).toUInt())->weight(query->value(3).toUInt(),query->value(2).toFloat());
        }
    }while(query->next());
    closeDb();
}
