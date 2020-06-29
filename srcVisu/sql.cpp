#include "sql.h"

Sql::Sql()
{

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
    qDebug()<<"ENTERIES"<<entries<<" "<<_q->first();
    return entries;
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
        throw("command failed to execut :\n"+command);
    }
    #if defined(Sql_Debug_Time)
        if(Sql_Debug_MinTime<tim.elapsed()){
        qDebug()<<tim.elapsed()<< " ms for:";
        qDebug()<<command;}
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
        mydb.transaction();
        //sqlcommandOpen("BEGIN TRANSACTION;");

    }
}

void Sql::connClose(){

    mydb.commit();
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


void Sql::isertIntoTable(string tableName,vector<string> columns,
                       vector<string> valuesName){
    vector<vector<string>> vec;
    vec.push_back(valuesName);
    isertIntoTable(tableName,columns,vec);
}

void Sql::isertIntoTable(string tableName,vector<string> columns,
    vector<vector<string>> valuesName){
    if(columns.size()!=valuesName[0].size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  values size "<<valuesName[0].size()<<" don't match";
    }
    string command;
    command ="INSERT INTO "+tableName;


        command +=" (";
        for(unsigned long long i=0;i<columns.size();i++){
            command +=columns[i];
            if(i<columns.size()-1){
                command += " , ";
            }
        }
        command += ") VALUES ";
        for(unsigned j=0;j<valuesName.size();j++){
            command +="(";
            for(unsigned long long i=0;i<valuesName[j].size();i++){
                command +=valuesName[j][i];
                if(i<valuesName[j].size()-1){
                    command += " , ";
                }
            }
        if(j<valuesName.size()-1){
            command += "),";
        }
    }
    command += ");";

    sqlcommandOpen(command);
}
