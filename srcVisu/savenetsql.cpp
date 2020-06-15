#include "savenetsql.h"

SaveNetSql::SaveNetSql(QString _path )
{
    netTable={  Net_Table_ID_Column,Net_Inputs_Column,Net_HiddenXs_Column,
                Net_HiddenYs_Column,Net_Outputs_Column,
                Net_Bias_Column,Net_Bias_Value_Column,Net_Average_Column,
                Net_Activation_Function_Column};

    neuronTable={ Neuron_Table_ID_Column,Neuron_Table_Net_ID_Column,
                  Neuron_ID_Column,Neuron_TYPE_Column};
    connectionTable={Connection_Table_ID_Column,Connection_Source_ID_Column,
                     Connection_Destination_ID_Column,Connection_Weight_Column,
                     Connection_Direction_Column};
    dbPath="C:/Users/SCHEH/Desktop/savedb.db";
    connOpen();
    //query=QSqlQuery(mydb);
    connClose();
    createDb();
    //connOpen();

}

void SaveNetSql::connOpen(){

    mydb = QSqlDatabase::addDatabase("QSQLITE","connction1");
    //mydb = QSqlDatabase::database("connction1");
    mydb.setDatabaseName(dbPath);

        if (!mydb.open())
        {
        mydb.setDatabaseName(dbPath);
          if (!mydb.open()){
          qDebug()<<"DB is not connected.";
        }
    }
    else
    {
       //qDebug()<<"DB is connected.";
    }
}

void SaveNetSql::connClose(){
    QString connection;
    connection = mydb.connectionName();
    mydb.close();
    //qDebug()<<" DB closed";
    mydb = QSqlDatabase();
    mydb.removeDatabase(connection);
}

void SaveNetSql::saveNet(Net* saveNet){
    int next=findFreeId(Net_Table_Name,Net_Table_ID_Column);
    //qDebug()<<" Free "<<next;
    saveNetId(saveNet,next);
    connClose();
}

void SaveNetSql::saveNetId(Net* saveNet,int id){
    //connOpen();
    QString tableName=Net_Table_Name;
    vector<QString> values;
    values.push_back(QString::number(id));
    values.push_back(QString::number(saveNet->get_inputNeurons()));
    values.push_back(QString::number(saveNet->get_hiddenNeuronsX()));
    values.push_back(QString::number(saveNet->get_hiddenNeuronsY()));
    values.push_back(QString::number(saveNet->get_outputNeurons()));
    values.push_back(QString::number(saveNet->get_bias()));
    values.push_back(QString::number(saveNet->get_biasValue()));
    values.push_back(QString::number(saveNet->get_enableAverage()));
    values.push_back(QString::number(saveNet->get_activationFunction()));
    isertIntoTable(tableName,netTable,values);

    vector<Neuron*> neuronList;
    neuronList=*saveNet->get_ptr_allNeurons();
    qDebug()<<neuronList.size();
    for(unsigned i=0;i<neuronList.size();i++){
        saveNeuron(neuronList[i],id);
    }

    Neuron input;
    input.set_TYPE(NeuronType::input);

    for(unsigned i=0;i<saveNet->get_inputNeurons();i++){
        input.set_ID(i);
        saveNeuron(&input,id);
    }

    if(saveNet->get_bias()){
        Neuron bias;
        bias.set_TYPE(NeuronType::bias);
        bias.set_ID(0);
        saveNeuron(&bias,id);
    }
    vector<Connection> connectionList;
    connectionList= saveNet->get_connectionList();
    for(unsigned i=0;i<connectionList.size();i++){
        saveConnection(&connectionList[i],id);
    }
    connClose();
}

Net* SaveNetSql::loadNet(int id){
    int input=0;
    int hiddenX=0;
    int hiddenY=0;
    int output=0;
    bool bias=0;
    bool average=0;
    float biasValue=0;
    int activationFunctionId=0;
    Activation activationFunction;
    QString command;
    command = "Select * FROM "+QString(Net_Table_Name)+" WHERE ";
    command += QString(Net_Table_ID_Column)+" = "+QString::number(id);
    //connOpen();
    sqlcommandi(command);

    if(countEnteries(&query)==1){
        query.first();
        input=query.value(Net_Inputs_Column).toInt();
        hiddenX=query.value(Net_HiddenXs_Column).toInt();
        hiddenY=query.value(Net_HiddenYs_Column).toInt();
        output=query.value(Net_Outputs_Column).toInt();
        bias=query.value(Net_Bias_Column).toBool();
        average=query.value(Net_Average_Column).toBool();
        biasValue=query.value(Net_Bias_Value_Column).toFloat();
        activationFunctionId=query.value(Net_Activation_Function_Column).toInt();
    }
    switch (activationFunctionId) {
        case Linear:
            activationFunction=Linear;
            break;
        case ReLu:
            activationFunction=ReLu;
            break;
        case Binary:
            activationFunction=Binary;
            break;
        case Gaussian:
            activationFunction=Gaussian;
            break;
        case Sigmoid:
            activationFunction=Sigmoid;
            break;
        default:
            activationFunction=Gaussian;
            break;
    }
    connClose();
    Net *net;
    net= new Net(0,input,hiddenX,hiddenY,output,bias,average,activationFunction);
    //Net net(get);
    return net;
}
void SaveNetSql::saveConnection(Connection* saveConnection,int netID){
    saveConnectionId(saveConnection,findFreeId(Connection_Table_Name,
                                        Connection_Table_ID_Column),netID);
}
void SaveNetSql::saveConnectionId(Connection* saveConnection,int id,int netID){
    QString tableName=Connection_Table_Name;
    vector<QString> values;
    values.push_back(QString::number(id));
    values.push_back(QString::number(findNeuron(&saveConnection->source_ID,netID)));
    values.push_back(QString::number(findNeuron(&saveConnection->destination_ID,netID)));
    values.push_back(QString::number(saveConnection->weight));
    values.push_back(QString::number(saveConnection->direction));
    isertIntoTable(tableName,connectionTable,values);
}
void SaveNetSql::saveNeuron(Neuron* saveNeuron,int netID){
    saveNeuronId(saveNeuron,findFreeId(Neuron_Table_Name,Neuron_Table_ID_Column),netID);
}

void SaveNetSql::saveNeuronId(Neuron* saveNeuron,int id,int netID){
    QString tableName=Neuron_Table_Name;
    vector<QString> values;
    values.push_back(QString::number(id));
    values.push_back(QString::number(netID));
    values.push_back(QString::number(saveNeuron->get_ID().ID));
    values.push_back(QString::number(saveNeuron->get_ID().TYPE));
    isertIntoTable(tableName,neuronTable,values);
    connClose();
}

void SaveNetSql::saveNetVec(vector<Net*> saveNet){
    for(unsigned long long i=0;i<saveNet.size();i++){
        //saveNet(saveNet[i]);
    }
}

void SaveNetSql::createDb(){
    vector<QString> types;
    types={ Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,
            Sql_Type_INT,Sql_Type_INT,
            Sql_Type_BOOL,Sql_Type_REAL,Sql_Type_BOOL,
            Sql_Type_INT};
    createTable(Net_Table_Name,netTable,types);

    types.clear();
    types={ Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,Sql_Type_INT};
    createTable(Neuron_Table_Name,neuronTable,types);
    types.clear();
    types={Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,Sql_Type_INT};
    createTable(Connection_Table_Name,connectionTable,types);

}

int SaveNetSql::countEnteries(QSqlQuery* _q){
    _q->first();
    int entries=0;
    if( _q->first()==true){
    do {
        ++entries;
    }while (_q->next());
    _q->first();
    }
    //qDebug()<<"ENTERIES"<<entries<<" "<<_q->first();
    return entries;
}

void SaveNetSql::sqlcommandi(QString command){
    connClose();
    connOpen();
    QSqlQuery _query(mydb);
    //qDebug()<<command;
    //qDebug()<<"";
    _query.exec(command);
    query=_query;
    _query.clear();
    query.first();
}

void SaveNetSql::sqlcommand(QString command){
    //connOpen();
    sqlcommandi(command);
    connClose();
}

bool SaveNetSql::createTable(QString tableName,vector<QString> columns,vector<QString> type){
    if(columns.size()!=type.size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  type size "<<type.size()<<" don't match";
        qDebug()<<type;
        return false;
    }
    QString command;
    command +="CREATE TABLE IF NOT EXISTS "+tableName+" (";
    for(unsigned long long i=0;i<type.size();i++){
        if(type[i]==Sql_Type_BOOL||type[i]==Sql_Type_TEXT
        ||type[i]==Sql_Type_INT||type[i]==Sql_Type_REAL){
            command +=columns[i]+" "+type[i];
            if(i<type.size()-1){
                command += " , ";
            }

        }
        else{
           qDebug()<<" sql Type "+type[i]+" unknown";
           return false;
        }
    }
    command += ");";
    sqlcommand(command);
    return true;
}

bool SaveNetSql::isertIntoTable(QString tableName,vector<QString> columns,
                                vector<QString> valuesName){

    if(columns.size()!=valuesName.size()){
        qDebug()<<" column size "<<columns.size()<<" and  values size "<<valuesName.size()<<" don't match";
        return false;
    }
    QString command;

    //command="SELECT "+idColumn+" FROM "+tableName+" WHERE "+idColumn+" ";
    //sqlcommandi(command);

    command +="INSERT INTO "+tableName+" (";
    for(unsigned long long i=0;i<columns.size();i++){
        command +=columns[i];
        if(i<columns.size()-1){
            command += " , ";
        }
    }
    command += ") VALUES(";
    for(unsigned long long i=0;i<valuesName.size();i++){
        command +=valuesName[i];
        if(i<valuesName.size()-1){
            command += " , ";
        }
    }

    command += ");";
    //qDebug()<<command;
    sqlcommandi(command);
    return true;
}

int SaveNetSql::findFreeId(QString tableName,QString columnId){
    QString command;
    command += "Select "+columnId+" FROM "+tableName;
    command += " ORDER BY "+columnId+" ASC";
    sqlcommandi(command);
    int value=0;
    if(countEnteries(&query)>0){
        do{
            //qDebug()<<query.value(columnId).toInt()<<" "<<value;
            if(query.value(columnId).toInt()==value){
                value=query.value(columnId).toInt()+1;

            }
        }while (query.next());
    }
    //qDebug()<<"new Value"<<value;
    connClose();
    return value;
}
int SaveNetSql::findNeuron(NeuronID *n, unsigned netID){
    QString command;
    QString placeholder=Neuron_Table_ID_Column;
    command="SELECT "+placeholder+" FROM "+Neuron_Table_Name+
            " WHERE "+Neuron_Table_Net_ID_Column+" = "+QString::number(netID)+
            " AND "+Neuron_ID_Column+" = "+QString::number(n->ID)+
            " AND "+Neuron_TYPE_Column" = "+QString::number(n->TYPE);
    sqlcommandi(command);
    if(query.first()){
        if(query.next()){
            qDebug()<<"Found multiple identical Neurons";
            return -1;
        }
        else{
            query.first();
            int result=query.value(Neuron_Table_ID_Column).toInt();
            connClose();
            return result;
        }

    }
    else{
        qDebug()<<command;
        qDebug()<<"No such Neuron found ID: "<<n->ID<<" Type: "<<n->TYPE+" NetID :"+netID;
        return -1;
    }
}
int SaveNetSql::findNeuron(Neuron *n, unsigned netID){
    NeuronID id=n->get_ID();
    return findNeuron(&id,netID);
}

