#ifndef SQL_H
#define SQL_H

#define ID_Column "rowid"

#define Sql_Type_INT "INT"
#define Sql_Type_TEXT "TEXT"
#define Sql_Type_BOOL "BOOL"
#define Sql_Type_REAL "REAL"

//#define Sql_Debug_Time
#define Sql_Debug_MinTime 1
//#define Sql_Debug_DB_Status


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
    QString dbPath;
    QSqlDatabase mydb;
    QSqlQuery query;
    void sqlcommandOpen(string command);
    void sqlcommand(string command);
    void isertIntoTable(string tableName,vector<string> columns,
                        vector<string> valuesName);
    void isertIntoTable(string tableName,vector<string> columns,
                        vector<vector<string>> valuesName);
    int countEnteries(QSqlQuery* _q);
};

#endif // SQL_H
