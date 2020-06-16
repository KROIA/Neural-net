#ifndef SAVENETSQL_H
#define SAVENETSQL_H

#define ID_Column "rowid"

#define Net_Table_Name "Net"

#define Net_Inputs_Column "InputsColumn"
#define Net_HiddenXs_Column "HiddenXsColumn"
#define Net_HiddenYs_Column "HiddenYsColumn"
#define Net_Outputs_Column "OutputsColumn"
#define Net_Bias_Column "BiasColumn"
#define Net_Bias_Value_Column "BiasValueColumn"
#define Net_Average_Column "AverageColumn"
#define Net_Activation_Function_Column "ActivationFunction"

#define Neuron_Table_Name "Neuron"


#define Neuron_Table_Net_ID_Column "NetID"
#define Neuron_ID_Column "NeuronID"
#define Neuron_TYPE_Column "NeuronType"

#define Connection_Table_Name "Connection"
#define Connection_Table_Net_ID_Column "NetID"
#define Connection_Source_ID_Column "SourceNeuronID"
#define Connection_Destination_ID_Column "DestinationNeuronID"
#define Connection_Weight_Column "ConWeight"
#define Connection_Direction_Column "direction"

#define Sql_Type_INT "INT"
#define Sql_Type_TEXT "TEXT"
#define Sql_Type_BOOL "BOOL"
#define Sql_Type_REAL "REAL"




#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QSqlQueryModel>
#include <QDebug>
#include <iostream>
#include <net.h>

#include <stdio.h>

using namespace std;
class SaveNetSql
{
public:
    SaveNetSql(string _path= "/bin/save.db");
    void connOpen();
    void connClose();
    QString dbPath;
    Net* loadNet(int id);
    void saveNet(Net* saveNet);
    void saveNetId(vector<Net*> saveNet,int id);
    void saveConnection(Connection saveConnection,int netID);
    void saveConnection(vector<Connection> saveConnection,int netID);

    void saveNeuron(Neuron* saveNeuron,int netID);
    void saveNetVec(vector<Net*> saveNet,int netID);
    void createDb();
    int countEnteries(QSqlQuery* _q);
private:
    QSqlDatabase mydb;
    QSqlQuery query;
    vector<string> netTable;
    vector<string> neuronTable;
    vector<string> connectionTable;
    void sqlcommandOpen(string command);
    void sqlcommand(string command);
    void saveNetOpen(vector<Net*> saveNet,int id);
    void saveNetOpen(Net* saveNet);
    void saveConnectionOpen(Connection saveConnection,int netID);
    void saveConnectionOpenVec(vector<Connection> saveConnection,int netID);
    void saveNeuronOpen(Neuron* saveNeuron,int netID);
    void saveNeuronOpenVec(vector<Neuron*> saveNeuron,int netID);
    bool createTable(string tableName,vector<string> columns,vector<string> type);
    void isertIntoTable(string tableName,vector<string> columns,
                        vector<string> valuesName);
    void isertIntoTableVec(string tableName,vector<string> columns,
                        vector<vector<string>> valuesName);
    int findFreeId(string tableName,string columnId);
    int findNeuron(Neuron *n, unsigned netID);
    int findNeuron(NeuronID *n, unsigned netID);
};

#endif // SAVENETSQL_H
