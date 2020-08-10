#ifndef SQL_H
#define SQL_H

#define ID_Column "rowid"

#define Sql_Type_INT "INT"
#define Sql_Type_TEXT "TEXT"
#define Sql_Type_BOOL "BOOL"
#define Sql_Type_REAL "REAL"
#define Sql_Type_DATE "DATE"
//#define Sql_Debug_Time
//#define Sql_Debug_MinTime 0
//#define Sql_Debug_Commit_Time
//#define Sql_Debug_DB_Status


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QSqlQueryModel>
#include <iostream>

enum SqlType{
    slqite   = 0,
    mysql  = 1,
};

using namespace std;

class Sql
{
public:
    Sql(SqlType _type);
    void connOpen();
    void connClose();
    QSqlDatabase db;
    //QSqlDatabase mydb;
    QSqlQuery *query;
    void sqlcommand(QString command);
    void insertIntoTable(QString tableName,vector<QString> columns,vector<QString> type,
                            vector<QString> valuesName);
        void insertIntoTable(QString tableName,vector<QString> columns,vector<QString> type,
                            vector<vector<QString>> valuesName);
    /*void insertIntoTable(QString tableName,vector<QString> columns,vector <QString> type,
                        vector<QVariant> value);
    void insertIntoTable(QString tableName,vector<QString> columns,vector <QString> type,
                        vector<vector<QVariant>> value);

    */
    void prepeareInsertTable(QString tableName,vector<QString> columns);
    bool createTable(QString tableName,vector<QString> columns,vector<QString> type);
    int countEnteries(QSqlQuery* _q);
    unsigned getFreeId(QString table,QString idColumn);

    bool dontClose;

    void setHostName(QString argHost);
    void setPort(unsigned argPort);
    void setDatabaseName(QString argDatabaseName);
    void setUserName(QString argUserName);
    void setPassword(QString argPassword);

    QString getHostName();
    unsigned getPort();
    QString getDatabaseName();
    QString getUserName();
    QString getPassword();

protected:

private:
    QString host;
    unsigned port;
    QString databaseName;
    QString userName;
    QString password;
    QString getBrackedString(vector<QString> data);
    QString getBrackedString(vector<vector<QString>> data);
    SqlType type;
};

#endif // SQL_H
