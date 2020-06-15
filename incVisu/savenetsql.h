#ifndef SAVENETSQL_H
#define SAVENETSQL_H

#define Net_Table_Name "Net"

#define Net_Table_ID_Column "NetID"
#define Net_Inputs_Column "InputsColumn"
#define Net_HiddenXs_Column "HiddenXsColumn"
#define Net_HiddenYs_Column "HiddenYsColumn"
#define Net_Outputs_Column "OutputsColumn"
#define Net_Bias_Column "BiasColumn"
#define Net_Bias_Value_Column "BiasValueColumn"
#define Net_Average_Column "AverageColumn"
#define Net_Activation_Function_Column "ActivationFunction"

#define Neuron_Table_Name "Neuron"

#define Neuron_Table_ID_Column "NeuronIDTable"
#define Neuron_Table_Net_ID_Column "NetID"
#define Neuron_ID_Column "NeuronID"
#define Neuron_TYPE_Column "NeuronType"

#define Connection_Table_Name "Connection"
#define Connection_Table_ID_Column "ConnectionIDTable"
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


using namespace std;
class SaveNetSql
{
public:
    SaveNetSql(QString _path= "/bin/save.db");
    void connOpen();
    void connClose();
    QString dbPath;
    Net* loadNet(int id);
    void saveNet(Net* saveNet);
    void saveNetId(Net* saveNet,int id);

    void saveConnection(Connection* saveConnection,int netID);
    void saveConnectionId(Connection* saveConnection,int id,int netID);

    void saveNeuron(Neuron* saveNeuron,int netID);
    void saveNeuronId(Neuron* saveNeuron,int id,int netID);
    void saveNetVec(vector<Net*> saveNet);
    void createDb();
    int countEnteries(QSqlQuery* _q);
private:
    QSqlDatabase mydb;
    QSqlQuery query;
    vector<QString> netTable;
    vector<QString> neuronTable;
    vector<QString> connectionTable;
    void sqlcommandi(QString command);
    void sqlcommand(QString command);
    bool createTable(QString tableName,vector<QString> columns,vector<QString> type);
    bool isertIntoTable(QString tableName,vector<QString> columns,
                        vector<QString> valuesName);
    int findFreeId(QString tableName,QString columnId);
    int findNeuron(Neuron *n, unsigned netID);
    int findNeuron(NeuronID *n, unsigned netID);
};

#endif // SAVENETSQL_H
