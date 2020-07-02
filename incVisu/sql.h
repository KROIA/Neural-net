#ifndef SQL_H
#define SQL_H

#define ID_Column "rowid"

#define Sql_Type_INT "INT"
#define Sql_Type_TEXT "TEXT"
#define Sql_Type_BOOL "BOOL"
#define Sql_Type_REAL "REAL"

//#define Sql_Debug_Time
//#define Sql_Debug_MinTime 1
#define Sql_Debug_Commit_Time
#define Sql_Debug_DB_Status


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QSqlQueryModel>
#include <iostream>



using namespace std;

class Sql
{
public:
    Sql();
    void connOpen();
    void connClose();

    QSqlDatabase mydb;
    QSqlQuery query;
    QString getDbPath();
    void setDbPath(QString dbFlieName);
    void sqlcommandOpen(string command);
    void sqlcommand(string command);
    void insertIntoTable(string tableName,vector<string> columns,
                        vector<string> valuesName);
    void insertIntoTable(string tableName,vector<string> columns,
                        vector<vector<string>> valuesName);
    bool createTable(string tableName,vector<string> columns,vector<string> type);
    int countEnteries(QSqlQuery* _q);

    bool dontClose;
protected:
    QString dbPath;
private:
    string getBrackedString(vector<string> data);
    string getBrackedString(vector<vector<string>> data);
};

#endif // SQL_H
