#ifndef SAVENETSQL_H
#define SAVENETSQL_H

#define Net_Table_Name "Net"
#define Net_Table_ID_Column "NetID"
#define Inputs_Column "InputsColumn"
#define HiddenXs_Column "HiddenXsColumn"
#define HiddenYs_Column "HiddenYsColumn"
#define Outputs_Column "OutputsColumn"

#define Neuron_Table_Name "Neuron"

#define Connection_Table_Name "Connection"

#define Sql_Type_INT "INT"
#define Sql_Type_TEXT "TEXT"
#define Sql_Type_BOOL "BOOL"

#include <QSqlDatabase>
#include <QSqlQuery>
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
    void saveNet(Net* saveNet);
    void saveNetVec(vector<Net*> saveNet);
    void createDb();
private:
    QSqlDatabase mydb;
    QSqlQuery query;
    vector<QString> netTable;
    void sqlcommandi(QString command);
    void sqlcommand(QString command);
    bool createTable(QString tableName,vector<QString> columns,vector<QString> type);
    bool isertIntoTable(QString tableName,vector<QString> columns,vector<QString> valuesName);
};

#endif // SAVENETSQL_H
