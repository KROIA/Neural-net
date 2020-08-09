#ifndef SAVENETSQL_H
#define SAVENETSQL_H


#define Net_Save_Table_Name "NetSave"

#define Net_Config_Column "NetConfig"

///////////////////////////////////////////////////////////////////

#define Net_Config_Table_Name "NetConfiguration"

#define Net_Config_Inputs_Column "InputsColumn"
#define Net_Config_HiddenXs_Column "HiddenXsColumn"
#define Net_Config_HiddenYs_Column "HiddenYsColumn"
#define Net_Config_Outputs_Column "OutputsColumn"
#define Net_Config_Bias_Column "BiasColumn"
#define Net_Config_Bias_Value_Column "BiasValueColumn"
#define Net_Config_Average_Column "AverageColumn"
#define Net_Config_Activation_Function_Column "ActivationFunction"

///////////////////////////////////////////////////////////////////

#define Net_Layout_Table_Name "NetLayout"

#define Net_Layout_Net_Config_Column "NetConfig"

///////////////////////////////////////////////////////////////////

#define Neuron_Config_Table_Name "NeuronConfig"

#define Neuron_Config_Table_Net_ID_Column "NetID"
#define Neuron_Config_ID_Column "NeuronID"
#define Neuron_Config_TYPE_Column "NeuronType"

///////////////////////////////////////////////////////////////////

#define Neuron_Layout_Table_Name "NeuronLayout"

#define Neuron_Layout_ABSPos "ABSPos"
#define Neuron_Layout_LayoutID "NetLayoutID"
#define Neuron_Layout_NeuronPosition_XRel_Column "XRelPos"
#define Neuron_Layout_NeuronPosition_YRel_Column "YRelPos"

///////////////////////////////////////////////////////////////////

#define Connection_Config_Table_Name "ConnectionConfig"

#define Connection_Config_Table_Net_ID_Column "NetID"
#define Connection_Save_ConId "ConnectionConfigId"
#define Connection_Config_Direction_Column "direction"
#define Connection_Config_Source_ID_Column "SourceNeuronID"
#define Connection_Config_Destination_ID_Column "DestinationNeuronID"

///////////////////////////////////////////////////////////////////

#define Connection_Save_Table_Name "ConnectionSave"

#define Connection_Save_Net_Save_Id "netSaveId"
#define Connection_Save_ConnectionId "ConnectionConfigId"
#define Connection_Save_Weight_Column "ConWeight"

///////////////////////////////////////////////////////////////////







#include <QDebug>
#include <iostream>
#include <net.h>
#include <sql.h>
#include <stdio.h>

using namespace std;

class SaveNetSql : public Sql
{
public:
    SaveNetSql(string _path= "/bin/save.db");
    ~SaveNetSql();

    //Net* loadNet(int id);
    void saveNet(Net* saveNet);
    void saveNet(vector<Net*> saveNet);
    void saveConnection(Connection saveConnection,int netID);
    void saveConnection(vector<Connection> saveConnection,int netID);
    void saveRelPos(QVector<qreal> relX,QVector<qreal> relY,Net *net);
    QVector<qreal> loadRelXPos(Net *net);
    QVector<qreal> loadRelYPos(Net *net);
    QVector<int> getNeuronLayout(Net *net);
    void saveNeuron(Neuron* saveNeuron,int netID);
    void saveNetVec(vector<Net*> saveNet,int netID);
    void createDb();
private:

    vector<QString> netSaveTable;
    vector<QString> netConfigTable;
    vector<QString> netLayoutTable;

    vector<QString> neuronConfigTable;
    vector<QString> neuronLayoutTable;

    vector<QString> connectionConfigTable;
    vector<QString> connectionSaveTable;

    vector<QString> netSaveTypeTable;
    vector<QString> netConfigTypeTable;
    vector<QString> netLayoutTypeTable;

    vector<QString> neuronConfigTypeTable;
    vector<QString> neuronLayoutTypeTable;

    vector<QString> connectionConfigTypeTable;
    vector<QString> connectionSaveTypeTable;

    vector<Net*> savedNet;
    vector<int> savedNetId;

    void saveNetOpen(vector<Net*> saveNet,int id);
    void saveNetOpen(Net* saveNet);
    int saveNetConfig(Net* net);
    void saveConnectionOpen(Connection saveConnection,int netID);
    void saveConnectionOpen(vector<Connection> saveConnection,int netID);
    void saveNeuronOpen(Neuron* saveNeuron,int netID);
    void saveNeuronOpen(vector<Neuron*> saveNeuron,int netID);


    int findNeuron(Neuron *n, unsigned netID);
    int findNeuron(NeuronID *n, unsigned netID);

    int checkConfig(Net* net);
    //int checkLatestLayout(Net* net);
    QVector<qreal> getRelVec(string column,Net* net);
};

#endif // SAVENETSQL_H
