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
    qDebug()<<command;
    qDebug()<<"";
    query->prepare(command);
    query->exec();
    query->first();
}
void NetDataBase::openDb(){
    mydb = QSqlDatabase::addDatabase("QSQLITE","connction1");
    //mydb = QSqlDatabase::database("connction1");
    mydb.setDatabaseName(_savepath.toString()+_dbfilename);
    if (!mydb.open())
          qDebug()<<"DB is not connected.";
    else
      qDebug()<<"DB is connected.";
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
        "ActivationFunction string NOT NULL);"
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
        "Neuron_id integer PRIMARY KEY, "
        "StartNeuron integer NOT NULL,"
        "EndNeuron integer NOT NULL,"
        "Weight real NOT NULL,"
        "Net_id integer NOT NULL,"
        //"FOREIGN KEY (StartNeuron,EndNeuron) REFERENCES Neuron(Neuron_id,Neuron_id) "
        "FOREIGN KEY (Net_id) REFERENCES Net(Net_id)"
        "ON DELETE CASCADE);"
        );
}

void NetDataBase::saveBackpropNet(BackpropNet *backPropNet){
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
            "'Sigmoid');");
    qDebug()<<"x"<<net->hiddenNeuronsX();

    for (unsigned int i=0;i<net->hiddenNeuronsX()+2;++i) {
        if(i==0){
            for (unsigned int j=0;j<net->inputNeurons();++j) {
                qDebug()<<"input:";
                saveNeuron(i,j);
            }
        }
        if(i==net->hiddenNeuronsX()+1){
            for (unsigned int j=0;j<net->outputNeurons();++j) {
                qDebug()<<"output:";
                saveNeuron(i,j);
            }
        }
        if(i>0&&i<net->hiddenNeuronsX()+1){
            qDebug()<<"hidden:";
            for (unsigned int j=0;j<net->hiddenNeuronsY();++j) {
                saveNeuron(i,j);
            }
        }
    }
}
void NetDataBase::saveNeuron(unsigned int posX, unsigned int posY){
    command("INSERT INTO Neuron(XPosition, YPosition, Net_id) "
            "VALUES("+QString::number(posX)+", "
            +QString::number(posY)+", "+QString::number(netId)+")");
}
void NetDataBase::saveConnection(Net *net){

}
