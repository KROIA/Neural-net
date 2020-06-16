#include "savenetsql.h"

SaveNetSql::SaveNetSql(string _path )
{
    netTable={  Net_Inputs_Column,Net_HiddenXs_Column,
                Net_HiddenYs_Column,Net_Outputs_Column,
                Net_Bias_Column,Net_Bias_Value_Column,Net_Average_Column,
                Net_Activation_Function_Column};

    neuronTable={Neuron_Table_Net_ID_Column,
                  Neuron_ID_Column,Neuron_TYPE_Column};
    connectionTable={Connection_Table_Net_ID_Column,
                     Connection_Source_ID_Column,
                     Connection_Destination_ID_Column,Connection_Weight_Column,
                     Connection_Direction_Column};
    dbPath="C:/Users/SCHEH/Desktop/savedb.db";
    //query=QSqlQuery(mydb);
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
        string command;
        command ="BEGIN TRANSACTION;";
        sqlcommandOpen(command);
        qDebug()<<" ";
       qDebug()<<"DB is connected.";
       qDebug()<<"BEGIN TRANSACTION;";
    }
}

void SaveNetSql::connClose(){
    sqlcommandOpen("COMMIT;");
    QString connection;
    connection = mydb.connectionName();
    mydb.close();
    qDebug()<<"COMMIT;";
    qDebug()<<" DB closed";
    qDebug()<<" ";
    mydb = QSqlDatabase();
    mydb.removeDatabase(connection);
}

void SaveNetSql::saveNet(Net* saveNet){
    //int next=findFreeId(Net_Table_Name,ID_Column);
    vector<Net*> net;
    net.push_back(saveNet);
    saveNetId(net,0);
    connClose();

}

void SaveNetSql::saveNetId(vector<Net*> saveNet,int id){
    connOpen();
    qDebug()<<"Start saving net";
    string command;
    for(unsigned j=0;j<saveNet.size();j++){
        string tableName=Net_Table_Name;
        vector<string> values;
        values.push_back(std::to_string(saveNet[j]->get_inputNeurons()));
        values.push_back(std::to_string(saveNet[j]->get_hiddenNeuronsX()));
        values.push_back(std::to_string(saveNet[j]->get_hiddenNeuronsY()));
        values.push_back(std::to_string(saveNet[j]->get_outputNeurons()));
        values.push_back(std::to_string(saveNet[j]->get_bias()));
        values.push_back(std::to_string(saveNet[j]->get_biasValue()));
        values.push_back(std::to_string(saveNet[j]->get_enableAverage()));
        values.push_back(std::to_string(saveNet[j]->get_activationFunction()));
        isertIntoTable(tableName,netTable,values);
        //sqlcommandOpen("COMMIT;");
        //sqlcommandOpen("select last_insert_rowid()");

        int netId=query.lastInsertId().toInt();


        Neuron input[saveNet[j]->get_inputNeurons()];
        vector<Neuron*> neuronVec;
        neuronVec=*saveNet[j]->get_ptr_allNeurons();

        for(unsigned i=0;i<saveNet[j]->get_inputNeurons();i++){
            input[i].set_ID(i);
            input[i].set_TYPE(NeuronType::input);
            neuronVec.push_back(&input[i]);
        }/*
        for(unsigned i=0;i<inputVec.size();i++){

        }*/
        Neuron bias;
        if(saveNet[j]->get_bias()){

            bias.set_TYPE(NeuronType::bias);
            bias.set_ID(0);
            neuronVec.push_back(&bias);
        }
        saveNeuronOpenVec(neuronVec,netId);

        //for(unsigned i=0;i<connectionList.size();i++){
        saveConnectionOpenVec(saveNet[j]->get_connectionList(),netId);
       //}
    }

    qDebug()<<"End saving net";
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
    string command;
    command = "Select * FROM "+string(Net_Table_Name)+" WHERE ";
    command += string(ID_Column)+" = "+std::to_string(id);
    //connOpen();
    sqlcommandOpen(command);

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
void SaveNetSql::saveConnection(Connection saveConnection,int netID){
    connOpen();
    saveConnectionOpen(saveConnection,netID);
    connClose();
}

void SaveNetSql::saveNeuron(Neuron* saveNeuron,int netID){
    connOpen();
        saveNeuronOpen(saveNeuron,netID);
    connClose();
}

void SaveNetSql::createDb(){
    vector<string> types;
    types={ Sql_Type_INT,Sql_Type_INT,
            Sql_Type_INT,Sql_Type_INT,
            Sql_Type_BOOL,Sql_Type_REAL,Sql_Type_BOOL,
            Sql_Type_INT};
    connOpen();
    createTable(Net_Table_Name,netTable,types);
    types.clear();
    types={ Sql_Type_INT,Sql_Type_INT,Sql_Type_INT};
    createTable(Neuron_Table_Name,neuronTable,types);
    types.clear();
    types={Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,Sql_Type_INT};
    createTable(Connection_Table_Name,connectionTable,types);
    connClose();
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
    qDebug()<<"ENTERIES"<<entries<<" "<<_q->first();
    return entries;
}

void SaveNetSql::sqlcommandOpen(string command){
    //connClose();
    //connOpen();
    QElapsedTimer t;
    t.start();
    QSqlQuery _query(mydb);
    if(!_query.exec(QString::fromStdString(command))){
        qDebug()<<"SQL ERROR command not working";
        qDebug()<<QString::fromStdString(command);
        qDebug()<<"";
    }
    query=_query;
    _query.clear();
    query.first();
    //qDebug()<<"This command took "<<t.elapsed();
    //qDebug()<< QString::fromStdString(command);
    //qDebug()<<"";
}

void SaveNetSql::sqlcommand(string command){
    connOpen();
    sqlcommandOpen(command);
    connClose();
}

void SaveNetSql::saveNetOpen(vector<Net*> saveNet,int id){

}

void SaveNetSql::saveConnectionOpen(Connection saveConnection,int netID){
    vector<Connection> vec;
    vec.push_back(saveConnection);
    saveConnectionOpenVec(vec,netID);
}
void SaveNetSql::saveConnectionOpenVec(vector<Connection> saveConnection,int netID){
    string tableName=Connection_Table_Name;
    vector<vector<string>> valuesVec;
    for(unsigned i=0;i<saveConnection.size();i++){
            vector<string> values;
            values.push_back(std::to_string(netID));
            values.push_back(std::to_string(findNeuron(&saveConnection[i].source_ID,netID)));
            values.push_back(std::to_string(findNeuron(&saveConnection[i].destination_ID,netID)));
            values.push_back(std::to_string(saveConnection[i].weight));
            values.push_back(std::to_string(saveConnection[i].direction));
            valuesVec.push_back(values);
    }
    isertIntoTableVec(tableName,connectionTable,valuesVec);
}

void SaveNetSql::saveNeuronOpen(Neuron* saveNeuron,int netID){
    vector<Neuron*> vec;
    vec.push_back(saveNeuron);
    saveNeuronOpenVec(vec,netID);

}
void SaveNetSql::saveNeuronOpenVec(vector<Neuron*> saveNeuron,int netID){
    string tableName=Neuron_Table_Name;
    vector<vector<string>> valuesVec;
    for(unsigned i=0;i<saveNeuron.size();i++){
            vector<string> values;
            values.push_back(std::to_string(netID));
            values.push_back(std::to_string(saveNeuron[i]->get_ID().ID));
            values.push_back(std::to_string(saveNeuron[i]->get_ID().TYPE));
            valuesVec.push_back(values);
    }
    isertIntoTableVec(tableName,neuronTable,valuesVec);
}
bool SaveNetSql::createTable(string tableName,vector<string> columns,vector<string> type){
    if(columns.size()!=type.size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  type size "<<type.size()<<" don't match";
        return false;
    }
    string command;
    command +="CREATE TABLE IF NOT EXISTS "+tableName+" (";
    //command +=ID_Column;
   // command +=" INT IDENTITY(1,1) PRIMARY KEY,";
    for(unsigned long long i=0;i<type.size();i++){
        if(type[i]==Sql_Type_BOOL||type[i]==Sql_Type_TEXT
        ||type[i]==Sql_Type_INT||type[i]==Sql_Type_REAL){
            command +=columns[i]+" "+type[i];
            if(i<type.size()-1){
                command += " , ";
            }
        }
        else{
           qDebug()<<" sql Type "+QString::fromStdString(type[i])+" unknown";
           return false;
        }
    }
    command+=");";
    sqlcommandOpen(command);
    return true;
}
void SaveNetSql::isertIntoTable(string tableName,vector<string> columns,
                       vector<string> valuesName){
    vector<vector<string>> vec;
    vec.push_back(valuesName);
    isertIntoTableVec(tableName,columns,vec);
}

void SaveNetSql::isertIntoTableVec(string tableName,vector<string> columns,
    vector<vector<string>> valuesName){
    if(columns.size()!=valuesName[0].size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  values size "<<valuesName[0].size()<<" don't match";
    }
    string command;

    //command="SELECT "+idColumn+" FROM "+tableName+" WHERE "+idColumn+" ";
    //sqlcommandOpen(command);

    command ="INSERT INTO "+tableName;

        command +=" (";
        for(unsigned long long i=0;i<columns.size();i++){
            command +=columns[i];
            if(i<columns.size()-1){
                command += " , ";
            }
        }
        command += ") VALUES";
        for(unsigned j=0;j<valuesName.size();j++){
            command +="(";
            for(unsigned long long i=0;i<valuesName[j].size();i++){
                command +=valuesName[j][i];
                if(i<valuesName[j].size()-1){
                    command += " , ";
                }
            }
        if(j<valuesName.size()-1){
            command += "),";
        }
    }
    command += ");";

    sqlcommandOpen(command);
}

int SaveNetSql::findFreeId(string tableName,string columnId){
    string command;
    command += "Select "+columnId+" FROM "+tableName;
    command += " ORDER BY "+columnId+" ASC";
    sqlcommandOpen(command);
    int value=0;
    int qV;
    if(countEnteries(&query)>0){
        do{
            qDebug()<<query.value(QString::fromStdString(columnId)).toInt()<<" "<<value;
            qV=query.value(QString::fromStdString(columnId)).toInt();
            if(qV==value){
                value=qV+1;
            }
        }while (query.next());
    }
    qDebug()<<"new Value"<<value;
    //connClose();
    return value;
}
int SaveNetSql::findNeuron(NeuronID *n, unsigned netID){
    string command;
    string placeholder=ID_Column;
    command="SELECT "+placeholder+" FROM "+Neuron_Table_Name+
            " WHERE "+Neuron_Table_Net_ID_Column+" = "+std::to_string(netID)+
            " AND "+Neuron_ID_Column+" = "+std::to_string(n->ID)+
            " AND "+Neuron_TYPE_Column" = "+std::to_string(n->TYPE);
    sqlcommandOpen(command);
    if(query.first()){
        if(query.next()){
            qDebug()<<"Found multiple identical Neurons";
            return -1;
        }
        else{
            query.first();
            int result=query.value(ID_Column).toInt();
            //connClose();
            return result;
        }

    }
    else{
        qDebug()<<QString::fromStdString(command);
        qDebug()<<"No such Neuron found ID: "<<n->ID<<" Type: "<<n->TYPE+" NetID :"+netID;
        return -1;
    }
    return 1;
}
int SaveNetSql::findNeuron(Neuron *n, unsigned netID){
    NeuronID id=n->get_ID();
    return findNeuron(&id,netID);
}

