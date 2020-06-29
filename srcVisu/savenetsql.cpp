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
void SaveNetSql::saveNet(Net* saveNet){
    //int next=findFreeId(Net_Table_Name,ID_Column);

    vector<Net*> net;
    net.push_back(saveNet);
    this->saveNet(net);
}

void SaveNetSql::saveNet(vector<Net*> saveNet){
    //QElapsedTimer tim;
    //tim.start();
    QElapsedTimer tim;
    tim.start();
    connOpen();
    //qDebug()<<"open Timer "<<tim.elapsed()<<" millis";
    //qDebug()<<"Start saving net";
    string command;
    tim.restart();
    for(unsigned j=0;j<saveNet.size();j++){
        string tableName=Net_Table_Name;
        vector<string> values;
        values.push_back(to_string(saveNet[j]->get_inputNeurons()));
        values.push_back(to_string(saveNet[j]->get_hiddenNeuronsX()));
        values.push_back(to_string(saveNet[j]->get_hiddenNeuronsY()));
        values.push_back(to_string(saveNet[j]->get_outputNeurons()));
        values.push_back(to_string(saveNet[j]->get_bias()));
        values.push_back(to_string(saveNet[j]->get_biasValue()));
        values.push_back(to_string(saveNet[j]->get_enableAverage()));
        values.push_back(to_string(saveNet[j]->get_activationFunction()));
        isertIntoTable(tableName,netTable,values);
        //sqlcommandOpen("COMMIT;");
        //sqlcommandOpen("select last_insert_rowid()");

        int netId=query.lastInsertId().toInt();
        //qDebug()<<"net Timer "<<tim.elapsed()<<" millis";
        tim.restart();
        Neuron input[saveNet[j]->get_inputNeurons()];
        vector<Neuron*> neuronVec;
        neuronVec=*saveNet[j]->get_ptr_allNeurons();

        for(unsigned i=0;i<saveNet[j]->get_inputNeurons();i++){
            input[i].set_ID(i);
            input[i].set_TYPE(NeuronType::input);
            neuronVec.push_back(&input[i]);
        }
        Neuron bias;
        if(saveNet[j]->get_bias()){

            bias.set_TYPE(NeuronType::bias);
            bias.set_ID(0);
            neuronVec.push_back(&bias);
        }
        saveNeuronOpen(neuronVec,netId);
        //qDebug()<<"neuron Timer "<<tim.elapsed()<<" millis";
        tim.restart();
        //for(unsigned i=0;i<connectionList.size();i++){
        saveConnectionOpen(saveNet[j]->get_connectionList(),netId);
        //qDebug()<<"connection Timer "<<tim.elapsed()<<" millis";
       //}
    }

    ////qDebug()<<"End saving net";
    tim.restart();
    connClose();
    //qDebug()<<"close Timer "<<tim.elapsed()<<" millis";
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
    command += string(ID_Column)+" = "+to_string(id);
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



void SaveNetSql::saveConnectionOpen(Connection saveConnection,int netID){
    vector<Connection> vec;
    vec.push_back(saveConnection);
    saveConnectionOpen(vec,netID);
}
void SaveNetSql::saveConnectionOpen(vector<Connection> saveConnection,int netID){
    string tableName=Connection_Table_Name;
    vector<vector<string>> valuesVec;
    for(unsigned i=0;i<saveConnection.size();i++){
            vector<string> values;
            values.push_back(to_string(netID));
            values.push_back(to_string(findNeuron(&saveConnection[i].source_ID,netID)));
            values.push_back(to_string(findNeuron(&saveConnection[i].destination_ID,netID)));
            values.push_back(to_string(saveConnection[i].weight));
            values.push_back(to_string(saveConnection[i].direction));
            valuesVec.push_back(values);
    }
    isertIntoTable(tableName,connectionTable,valuesVec);
}

void SaveNetSql::saveNeuronOpen(Neuron* saveNeuron,int netID){
    vector<Neuron*> vec;
    vec.push_back(saveNeuron);
    saveNeuronOpen(vec,netID);

}
void SaveNetSql::saveNeuronOpen(vector<Neuron*> saveNeuron,int netID){
    string tableName=Neuron_Table_Name;
    vector<vector<string>> valuesVec;
    for(unsigned i=0;i<saveNeuron.size();i++){
            vector<string> values;
            values.push_back(to_string(netID));
            values.push_back(to_string(saveNeuron[i]->get_ID().ID));
            values.push_back(to_string(saveNeuron[i]->get_ID().TYPE));
            valuesVec.push_back(values);
    }
    isertIntoTable(tableName,neuronTable,valuesVec);
}
bool SaveNetSql::createTable(string tableName,vector<string> columns,vector<string> type){
    if(columns.size()!=type.size()){
        //qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  type size "<<type.size()<<" don't match";
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
           //qDebug()<<" sql Type "+type[i]+" unknown";
           return false;
        }
    }
    command+=");";
    sqlcommandOpen(command);
    return true;
}



int SaveNetSql::findNeuron(NeuronID *n, unsigned netID){
    string command;
    string placeholder=ID_Column;
    command="SELECT "+placeholder+" FROM "+Neuron_Table_Name+
            " WHERE "+Neuron_Table_Net_ID_Column+" = "+to_string(netID)+
            " AND "+Neuron_ID_Column+" = "+to_string(n->ID)+
            " AND "+Neuron_TYPE_Column" = "+to_string(n->TYPE);
    sqlcommandOpen(command);
    if(query.first()){
        if(query.next()){
            //qDebug()<<"Found multiple identical Neurons";
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
        //qDebug()<<command;
        //qDebug()<<"No such Neuron found ID: "<<n->ID<<" Type: "<<n->TYPE+" NetID :"+netID;
        return -1;
    }
    return 1;
}

int SaveNetSql::findNeuron(Neuron *n, unsigned netID){
    NeuronID id=n->get_ID();
    return findNeuron(&id,netID);
}

