#include "savenetsql.h"

SaveNetSql::SaveNetSql(string _path )
{
    netLayoutTable={  Net_Inputs_Column,Net_HiddenXs_Column,
                Net_HiddenYs_Column,Net_Outputs_Column,
                Net_Bias_Column,Net_Bias_Value_Column,Net_Average_Column,
                Net_Activation_Function_Column};
    netTable={Net_Layout_Column};
    neuronTable={Neuron_Table_Net_ID_Column,
                  Neuron_ID_Column,Neuron_TYPE_Column};

    neuronLayoutTable={NeuronLayout_ABSPos,
                        NeuronLayout_LayoutID,
                       NeuronLayout_NeuronPosition_XRel_Column,
                      NeuronLayout_NeuronPosition_YRel_Column};

    connectionTable={Connection_Table_Net_ID_Column,
                     Connection_Source_ID_Column,
                     Connection_Destination_ID_Column,Connection_Weight_Column,
                     Connection_Direction_Column};
    dbPath="C:/Users/SCHEH/Desktop/savedb.db";
    //query=QSqlQuery(mydb);
    createDb();
    //connOpen();
    dontClose=false;
}

void SaveNetSql::createDb(){
    vector<string> types;
    types={ Sql_Type_INT,Sql_Type_INT,
            Sql_Type_INT,Sql_Type_INT,
            Sql_Type_BOOL,Sql_Type_REAL,Sql_Type_BOOL,
            Sql_Type_INT};
    connOpen();
    createTable(Net_Layout_Table_Name,netLayoutTable,types);

    types.clear();
    types={Sql_Type_INT};
    createTable(Net_Table_Name,netTable,types);
    types.clear();
    types={Sql_Type_INT,Sql_Type_INT,Sql_Type_REAL,Sql_Type_REAL};
    createTable(NeuronLayout_Table_Name,neuronLayoutTable,types);

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
    dontClose=true;
    //qDebug()<<"open Timer "<<tim.elapsed()<<" millis";
    string command;
    tim.restart();
    int layoutId=checkLatestLayout(saveNet[0]);
    if(layoutId<0){
           layoutId=saveNetLayout(saveNet[0]);
    }
    int netId;
    for(unsigned j=0;j<saveNet.size();j++){
        command="Insert INTO ";
        command+=Net_Table_Name;
        command+=" ( ";
        command+=Net_Layout_Column;
        command+=") VALUES("+to_string(layoutId)+") ";
        sqlcommandOpen(command);
        netId=query.lastInsertId().toInt();
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
    dontClose=false;
    connClose();
    //qDebug()<<"close Timer "<<tim.elapsed()<<" millis";
}

int SaveNetSql::saveNetLayout(Net* net){
    if(!dontClose)
        connOpen();
    string tableName=Net_Layout_Table_Name;
    vector<string> values;
    values.push_back(to_string(net->get_inputNeurons()));
    values.push_back(to_string(net->get_hiddenNeuronsX()));
    values.push_back(to_string(net->get_hiddenNeuronsY()));
    values.push_back(to_string(net->get_outputNeurons()));
    values.push_back(to_string(net->get_bias()));
    values.push_back(to_string(net->get_biasValue()));
    values.push_back(to_string(net->get_enableAverage()));
    values.push_back(to_string(net->get_activationFunction()));
    isertIntoTable(tableName,netLayoutTable,values);
    int res;
    res= query.lastInsertId().toInt();
    if(!dontClose)
        connClose();
    return res;
}

Net* SaveNetSql::loadNet(int id){
    int input=0;
    int hiddenX=0;
    int hiddenY=0;
    int output=0;
    bool bias=0;
    bool average=0;
    float biasValue=0;
    Activation activationFunction=Linear;
    string command;
    command = "Select * FROM "+string(Net_Table_Name)+" WHERE ";
    command += string(ID_Column)+" = "+to_string(id);
    if(!dontClose)
        connOpen();
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
        activationFunction=Activation(query.value(Net_Activation_Function_Column).toInt());
    }
    if(!dontClose)
        connClose();
    Net *net;
    net= new Net(0,input,hiddenX,hiddenY,output,bias,average,activationFunction);
    //Net net(get);
    return net;
}
void SaveNetSql::saveConnection(Connection saveConnection,int netID){
    if(!dontClose)
        connOpen();
    saveConnectionOpen(saveConnection,netID);
    if(!dontClose)
        connClose();
}

void SaveNetSql::saveNeuron(Neuron* saveNeuron,int netID){
    if(!dontClose)
        connOpen();
        saveNeuronOpen(saveNeuron,netID);
    if(!dontClose)
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

void SaveNetSql::saveRelPos(QVector<qreal> relX,QVector<qreal> relY,Net *net){
    connOpen();
    dontClose=true;
    int netID;
    //netID=checkLayout(net);
    string tableName=NeuronLayout_Table_Name;
    //if(netID<0){

    netID=saveNetLayout(net);
    //}
    qDebug()<<netID;
    vector<vector<string>> data;
    vector<string> dataRow;
    qDebug()<<relX.size();
    for(int i=0;i<relX.size();i++){
        dataRow.clear();

        dataRow.push_back(to_string(i));
        dataRow.push_back(to_string(netID));
        dataRow.push_back(to_string(relX[i]));
        dataRow.push_back(to_string(relY[i]));
        data.push_back(dataRow);
    }
    //qDebug()<<"3";
    //qDebug()<<neuronLayoutTable.size()<<data.size()<<data[0].size();
    isertIntoTable(tableName,neuronLayoutTable,data);
    //qDebug()<<"4";
    dontClose=false;
    connClose();
}

QVector<qreal> SaveNetSql::loadRelXPos(Net *net){
    return getRelVec(NeuronLayout_NeuronPosition_XRel_Column,net);
}

QVector<qreal> SaveNetSql::loadRelYPos(Net *net){
    return getRelVec(NeuronLayout_NeuronPosition_YRel_Column,net);
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

bool SaveNetSql::checkLayout(int layoutId,Net* net){
    if(!dontClose)
        connOpen();
    QVector<int> vec;
    vec=checkLayout(net);
    if(vec.size()>0){
       string command;
       command+="SELECT COUNT(*) FROM ";
       command+=NeuronLayout_Table_Name;
       command+=" WHERE ";
       command+=NeuronLayout_LayoutID;
       command+=" = ";
       command+=to_string(layoutId);
       sqlcommandOpen(command);
       if(query.first()){
           unsigned int neuron;
           neuron=0;
           if(net->get_bias()){
               neuron+=net->get_hiddenNeurons()+1;
           }
           neuron+=net->get_neurons();
           neuron+=net->get_inputNeurons();
           if(query.value(0).toUInt()==neuron){
              return true;
           }
       }
    }
    if(!dontClose)
        connClose();
    return false;
}

QVector<int> SaveNetSql::checkLayout(Net* net){
    string command;
    if(!dontClose)
        connOpen();
    command= "SELECT ";
    command+=ID_Column ;
    command+=" FROM ";
    command+= Net_Layout_Table_Name;
    command+= " WHERE ";
    command+=to_string(net->get_inputNeurons())+" ="+ Net_Inputs_Column+" AND ";
    command+=to_string(net->get_hiddenNeuronsX())+" = "+Net_HiddenXs_Column+" AND ";
    command+=to_string(net->get_hiddenNeuronsY())+" = "+Net_HiddenYs_Column+" AND ";
    command+=to_string(net->get_outputNeurons())+" = "+Net_Outputs_Column+" AND ";
    command+=to_string(net->get_bias())+" = "+Net_Bias_Column+" AND ";
    command+=to_string(net->get_biasValue())+" = "+Net_Bias_Value_Column+" AND ";
    command+=to_string(net->get_enableAverage())+" = "+Net_Average_Column+" AND ";
    command+=to_string(net->get_activationFunction())+" = "+Net_Activation_Function_Column+" ; ";
    sqlcommandOpen(command);
    QVector<int> res;
    query.first();
    res.push_back(query.value(ID_Column).toInt());
    while(query.next()){
        res.push_back(query.value(ID_Column).toInt());
    }
    if(!dontClose)
        connClose();
    return res;
}

int SaveNetSql::checkLatestLayout(Net* net){
    QVector<int> vec;
    vec=checkLayout(net);
    if(vec.size()>0){
        return vec.last();
    }
    else{
        return  -1;
    }
}

QVector<qreal> SaveNetSql::getRelVec(string column,Net* net){
    int id;
    connOpen();
    dontClose=true;
    QVector<int> idVec;
    QVector<qreal> value;
    idVec=checkLayout(net);


    for (int i=idVec.size()-1;i>=0;i--) {

        if(checkLayout(idVec[i],net)){
            qDebug()<<idVec[i];
            string command;
            command= "SELECT ";
            command+=column;
            command+=" FROM ";
            command+=NeuronLayout_Table_Name;
            command+=" WHERE ";
            command+=NeuronLayout_LayoutID;
            command+=" = ";
            command+=to_string(idVec[i]);
            sqlcommandOpen(command);
            if(query.first()){

               value.push_back(query.value(0).toReal());
               while(query.next()){
                   value.push_back(query.value(0).toReal());
               }
               dontClose=false;
               connClose();
               return value;
            }
        }
    }
    dontClose=false;
    connClose();
    return value;
}
