#include "savenetsql.h"

SaveNetSql::SaveNetSql(string _path )
{   netSaveTable={
        Net_Config_Column
    };

    netConfigTable={
        Net_Config_Inputs_Column,
        Net_Config_HiddenXs_Column,
        Net_Config_HiddenYs_Column,
        Net_Config_Outputs_Column,
        Net_Config_Bias_Column,
        Net_Config_Bias_Value_Column,
        Net_Config_Average_Column,
        Net_Config_Activation_Function_Column
    };

    netLayoutTable={
        Net_Layout_Net_Config_Column,
    };

    neuronConfigTable={

        Neuron_Config_Table_Net_ID_Column,
        Neuron_Config_ID_Column,
        Neuron_Config_TYPE_Column
    };

    neuronLayoutTable={
        Neuron_Layout_ABSPos,
        Neuron_Layout_LayoutID,
        Neuron_Layout_NeuronPosition_XRel_Column,
        Neuron_Layout_NeuronPosition_YRel_Column
    };


    connectionConfigTable={
        Connection_Config_Table_Net_ID_Column,
        Connection_Save_ConId,
        Connection_Config_Direction_Column,
        Connection_Config_Source_ID_Column,
        Connection_Config_Destination_ID_Column

    };

    connectionSaveTable={
        Connection_Save_Net_Save_Id,
        Connection_Save_ConnectionId,
        Connection_Save_Weight_Column
    };

    dbPath="savedb.db";
    //query=QSqlQuery(mydb);
    createDb();
    //connOpen();
    dontClose=false;
}

void SaveNetSql::createDb(){
    connOpen();
    vector<string> types;
    types={ Sql_Type_INT};
    createTable(Net_Save_Table_Name,netSaveTable,types);

    types={ Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_BOOL,
            Sql_Type_REAL,
            Sql_Type_BOOL,
            Sql_Type_INT};
    createTable(Net_Config_Table_Name,netConfigTable,types);

    types={ Sql_Type_INT};
    createTable(Net_Layout_Table_Name,netLayoutTable,types);

    types={ Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_INT};
    createTable(Neuron_Config_Table_Name,neuronConfigTable,types);

    types={ Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_REAL,
            Sql_Type_REAL};
    createTable(Neuron_Layout_Table_Name,neuronLayoutTable,types);

    types={ Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_INT};
    createTable(Connection_Config_Table_Name,connectionConfigTable,types);

    types={ Sql_Type_INT,
            Sql_Type_INT,
            Sql_Type_REAL};
    createTable(Connection_Save_Table_Name,connectionSaveTable,types);
    connClose();
}
void SaveNetSql::saveNet(Net* saveNet){
    //int next=findFreeId(Net_Table_Name,ID_Column);

    vector<Net*> net;
    net.push_back(saveNet);
    this->saveNet(net);
}

void SaveNetSql::saveNet(vector<Net*> saveNet){
    connOpen();
    dontClose=true;
    string command;
    vector<int> netId;
    for(unsigned i=0;i<saveNet.size();i++){
            int netConfigId=checkConfig(saveNet[i]);
            vector<string> values;
            values.push_back(to_string(netConfigId));
            insertIntoTable(Net_Save_Table_Name,netSaveTable,values);
            netId.push_back(query.lastInsertId().toInt());
    }

    for(unsigned j=0;j<saveNet.size();j++){
        vector<double*> *saveGenom;
        saveGenom=saveNet[j]->get_ptr_genom();

        vector<vector<string>> valuesVec;
        for(unsigned int i=0;i<saveNet[j]->get_genomsize();i++){
            vector<string> valuesGenom;
            valuesGenom.push_back(to_string(netId[j]));
            valuesGenom.push_back(to_string(i));
            valuesGenom.push_back(to_string(*(*saveGenom)[i]));
            valuesVec.push_back(valuesGenom);
        }
        insertIntoTable(Connection_Save_Table_Name,connectionSaveTable,valuesVec);
    }
    dontClose=false;
    connClose();
}

int SaveNetSql::saveNetConfig(Net* net){
    if(!dontClose)
        connOpen();
    string tableName=Net_Config_Table_Name;
    vector<string> values;

    int netId;
    values.push_back(to_string(net->get_inputNeurons()));
    values.push_back(to_string(net->get_hiddenNeuronsX()));
    values.push_back(to_string(net->get_hiddenNeuronsY()));
    values.push_back(to_string(net->get_outputNeurons()));
    values.push_back(to_string(net->get_bias()));
    values.push_back(to_string(net->get_biasValue()));
    values.push_back(to_string(net->get_enableAverage()));
    values.push_back(to_string(net->get_activationFunction()));

    insertIntoTable(tableName,netConfigTable,values);

    netId=query.lastInsertId().toInt();
    Neuron input[net->get_inputNeurons()];
    vector<Neuron*> neuronVec;
    neuronVec=*net->get_ptr_allNeurons();
    for(unsigned i=0;i<net->get_inputNeurons();i++){
        input[i].set_ID(i);
        input[i].set_TYPE(NeuronType::input);
        neuronVec.push_back(&input[i]);
    }
    Neuron bias;
    if(net->get_bias()){
        bias.set_TYPE(NeuronType::bias);
        bias.set_ID(0);
        neuronVec.push_back(&bias);
    }
    saveNeuronOpen(neuronVec,netId);
    saveConnectionOpen(net->get_connectionList(),netId);
    int res;
    res= query.lastInsertId().toInt();
    if(!dontClose)
        connClose();
    return res;
}

/*Net* SaveNetSql::loadNet(int id){
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
}*/
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
    string tableName=Connection_Config_Table_Name;
    vector<vector<string>> valuesVec;
    for(unsigned i=0;i<saveConnection.size();i++){
            vector<string> values;
            values.push_back(to_string(netID));
            values.push_back(to_string(i));
            values.push_back(to_string(findNeuron(&saveConnection[i].source_ID,netID)));
            values.push_back(to_string(findNeuron(&saveConnection[i].destination_ID,netID)));
            values.push_back(to_string(saveConnection[i].direction));
            valuesVec.push_back(values);
    }
    insertIntoTable(tableName,connectionConfigTable,valuesVec);
}

void SaveNetSql::saveRelPos(QVector<qreal> relX,QVector<qreal> relY,Net *net){
    connOpen();
    dontClose=true;
    int netConfigId;
    //netID=checkLayout(net);
    string tableName=Neuron_Layout_Table_Name;
    //if(netID<0){

    netConfigId=checkConfig(net);

    if(netConfigId<0){
           netConfigId=saveNetConfig(net);
    }
    vector<string> valuesVec;
    valuesVec.push_back(to_string(netConfigId));
    insertIntoTable(Net_Layout_Table_Name,netLayoutTable,valuesVec);
    vector<vector<string>> data;
    vector<string> dataRow;
    int layoutId;
    layoutId=query.lastInsertId().toInt();
    for(int i=0;i<relX.size();i++){
        dataRow.clear();
        dataRow.push_back(to_string(i));
        dataRow.push_back(to_string(layoutId));
        dataRow.push_back(to_string(relX[i]));
        dataRow.push_back(to_string(relY[i]));
        data.push_back(dataRow);
    }
    //qDebug()<<"3";
    //qDebug()<<neuronLayoutTable.size()<<data.size()<<data[0].size();
    insertIntoTable(tableName,neuronLayoutTable,data);
    //qDebug()<<"4";
    dontClose=false;
    connClose();
}

QVector<qreal> SaveNetSql::loadRelXPos(Net *net){
    return getRelVec(Neuron_Layout_NeuronPosition_XRel_Column,net);
}

QVector<qreal> SaveNetSql::loadRelYPos(Net *net){
    return getRelVec(Neuron_Layout_NeuronPosition_YRel_Column,net);
}

void SaveNetSql::saveNeuronOpen(Neuron* saveNeuron,int netID){
    vector<Neuron*> vec;
    vec.push_back(saveNeuron);
    saveNeuronOpen(vec,netID);

}
void SaveNetSql::saveNeuronOpen(vector<Neuron*> saveNeuron,int netID){
    string tableName=Neuron_Config_Table_Name;
    vector<vector<string>> valuesVec;
    for(unsigned i=0;i<saveNeuron.size();i++){
            vector<string> values;
            values.push_back(to_string(netID));
            values.push_back(to_string(saveNeuron[i]->get_ID().ID));
            values.push_back(to_string(saveNeuron[i]->get_ID().TYPE));
            valuesVec.push_back(values);
    }
    insertIntoTable(tableName,neuronConfigTable,valuesVec);
}

int SaveNetSql::findNeuron(NeuronID *n, unsigned netID){
    string command;
    string placeholder=ID_Column;
    command="SELECT "+placeholder+" FROM "+Neuron_Config_Table_Name+
            " WHERE "+Neuron_Config_Table_Net_ID_Column+" = "+to_string(netID)+
            " AND "+Neuron_Config_ID_Column+" = "+to_string(n->ID)+
            " AND "+Neuron_Config_TYPE_Column" = "+to_string(n->TYPE);
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

int SaveNetSql::checkConfig(Net* net){
    string command;
    int res=-1;
    if(!dontClose)
        connOpen();

    for(unsigned i=0;i<savedNet.size();i++){
        if(savedNet[i]==net){
            return savedNetId[i];
        }
    }
    command= "SELECT ";
    command+=ID_Column ;
    command+=" FROM ";
    command+= Net_Config_Table_Name;
    command+= " WHERE ";
    command+=to_string(net->get_inputNeurons())+" ="+ Net_Config_Inputs_Column+" AND ";
    command+=to_string(net->get_hiddenNeuronsX())+" = "+Net_Config_HiddenXs_Column+" AND ";
    command+=to_string(net->get_hiddenNeuronsY())+" = "+Net_Config_HiddenYs_Column+" AND ";
    command+=to_string(net->get_outputNeurons())+" = "+Net_Config_Outputs_Column+" AND ";
    command+=to_string(net->get_bias())+" = "+Net_Config_Bias_Column+" AND ";
    command+=to_string(net->get_biasValue())+" = "+Net_Config_Bias_Value_Column+" AND ";
    command+=to_string(net->get_enableAverage())+" = "+Net_Config_Average_Column+" AND ";
    command+=to_string(net->get_activationFunction())+" = "+Net_Config_Activation_Function_Column+" ; ";
    sqlcommandOpen(command);

    if(query.first()){
        res=query.value(ID_Column).toInt();
    }
    if(res<0){
           res=saveNetConfig(net);
    }
    savedNet.push_back(net);
    savedNetId.push_back(res);
    return res;
}

/*int SaveNetSql::checkLatestLayout(Net* net){
    QVector<int> vec;
    vec=checkLayout(net);
    if(vec.size()>0){
        return vec.last();
    }
    else{
        return  -1;
    }
}*/

QVector<qreal> SaveNetSql::getRelVec(string column,Net* net){
    connOpen();
    dontClose=true;
    QVector<qreal> value;
    int id;
    id=checkConfig(net);
    string command;
    command= "SELECT ";
    command+=ID_Column;
    command+=" FROM ";
    command+=Net_Layout_Table_Name;
    command+=" WHERE ";
    command+=Net_Layout_Net_Config_Column;
    command+=" = ";
    command+=to_string(id);
    sqlcommandOpen(command);
    if(query.last()){
        int layoutNetId;
        layoutNetId=query.value(ID_Column).toInt();
        command= "SELECT ";
        command+=column;
        command+=" FROM ";
        command+=Neuron_Layout_Table_Name;
        command+=" WHERE ";
        command+=Neuron_Layout_LayoutID;
        command+=" = ";
        command+=to_string(layoutNetId);
        command+= " ORDER BY ";
        command+=Neuron_Layout_ABSPos;
        command+= " ASC; ";
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
    dontClose=false;
    connClose();
    return value;
}
