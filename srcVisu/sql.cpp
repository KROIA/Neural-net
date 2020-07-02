#include "sql.h"

Sql::Sql()
{

}


void Sql::connOpen(){

    mydb = QSqlDatabase::addDatabase("QSQLITE","connction1");
    //mydb = QSqlDatabase::database("connction1");
    mydb.setDatabaseName(dbPath);

        if (!mydb.open())
        {
        mydb.setDatabaseName(dbPath);
          if (!mydb.open()){
            #if defined (Sql_Debug_DB_Status)
                   qDebug()<<" ";
                   qDebug()<<"DB failed to connect";
            #endif
        }
    }
    else
    {
#if defined (Sql_Debug_DB_Status)
    qDebug()<<" ";
    qDebug()<<"DB is connected.";
#endif
#if defined (Sql_Debug_Commit_Time)
    QElapsedTimer tim;
    tim.start();
#endif
        mydb.transaction();
#if defined (Sql_Debug_Commit_Time)
    qDebug()<<tim.elapsed()<< " ms for transaction";
#endif
        //sqlcommandOpen("BEGIN TRANSACTION;");

    }
}

void Sql::connClose(){

#if defined (Sql_Debug_Commit_Time)
    QElapsedTimer tim;
    tim.start();
#endif
        mydb.commit();
#if defined (Sql_Debug_Commit_Time)
    qDebug()<<tim.elapsed()<< " ms for COMMIT";
#endif
    //sqlcommandOpen("END TRANSACTION;");
    QString connection;
    connection = mydb.connectionName();
    mydb.close();
    #if defined (Sql_Debug_DB_Status)
           qDebug()<<" ";
           qDebug()<<"DB connection closed";
    #endif
    mydb = QSqlDatabase();
    mydb.removeDatabase(connection);
}

void Sql::setDbPath(QString dbFlieName){
    dbPath=dbFlieName;
}

void Sql::sqlcommandOpen(string command){
    //connClose();
    //connOpen();
    try {
    #if defined(Sql_Debug_Time)
        QElapsedTimer tim;
        tim.start();
    #endif
    QSqlQuery _query(mydb);

    if(!_query.exec(QString::fromStdString(command))){
        throw("command failed to execut : "+command);
    }
    //qDebug()<<QString::fromStdString(command);
    #if defined(Sql_Debug_Time)
        if(Sql_Debug_MinTime<tim.elapsed()){
        qDebug()<<tim.elapsed()<< " ms for:";
        qDebug()<<QString::fromStdString(command);}
    #endif
    query=_query;
    _query.clear();
    query.first();
    }
    catch (string error) {
        qDebug()<<"SQL ERROR sqlcommandOpen()::"+QString::fromStdString(error);
        //throw "sqlcommandOpen()::"+error;
        }
}

void Sql::sqlcommand(string command){
    connOpen();
    sqlcommandOpen(command);
    connClose();
}



void Sql::insertIntoTable(string tableName,vector<string> columns,
    vector<string> valuesName){
    vector<vector<string>> vec;
    vec.push_back(valuesName);
    insertIntoTable(tableName,columns,vec);
}

void Sql::insertIntoTable(string tableName,vector<string> columns,
    vector<vector<string>> valuesName){
    if(!dontClose)
        connOpen();
    if(columns.size()!=valuesName[0].size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  values size "<<valuesName[0].size()<<" don't match";
    }
    string command;
    command ="INSERT INTO "+tableName;
    command +=getBrackedString(columns);
    command += " VALUES ";
    command +=getBrackedString(valuesName);
    command += ";";
    sqlcommandOpen(command);
    if(!dontClose)
        connClose();
}

bool Sql::createTable(string tableName,vector<string> columns,vector<string> type){
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

int Sql::countEnteries(QSqlQuery* _q){
    _q->first();
    int entries=0;
    if( _q->first()==true){
    do {
        ++entries;
    }while (_q->next());
    _q->first();
    }
    //qDebug()<<"ENTERIES"<<entries<<" "<<_q->first();
    return entries;
}

string Sql::getBrackedString(vector<string> data){
    string res;
    res ="(";
    for(unsigned long long i=0;i<data.size();i++){
        res +=data[i];
        if(i<data.size()-1){
            res += " , ";
        }
    }
    res+=") ";
    return res;
}

string Sql::getBrackedString(vector<vector<string>> data){
    string res;
    for(unsigned long long i=0;i<data.size();i++){
        res +=this->getBrackedString(data[i]);
        if(i<data.size()-1){
            res += " , ";
        }
    }
    return res;
}

