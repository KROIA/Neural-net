#include "sql.h"

Sql::Sql(SqlType _type)
{
    type=_type;
}

void Sql::connOpen(){  
    //mydb = QSqlDatabase::database("connction1");

    if(type==slqite){
        db=QSqlDatabase::addDatabase("QSQLITE","connction1");
    }
    if(type==mysql){
        db=QSqlDatabase::addDatabase("QMYSQL","");
    }
        if (!db.isOpen())
        {
            db.setDatabaseName(databaseName);
            if(type==mysql){
                db.setHostName(host);
                db.setPort(int(port));
                db.setUserName(userName);
                db.setPassword(password);
            }

            if (!db.open()){
            #if defined (Sql_Debug_DB_Status)
                   qDebug()<<" ";
                   qDebug()<<"DB failed to connect";
                   qDebug()<<db.lastError();
            #endif
        }
        query = new QSqlQuery(db);
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
        db.transaction();
#if defined (Sql_Debug_Commit_Time)
    qDebug()<<tim.elapsed()<< " ms for transaction";
#endif
        //sqlcommand("BEGIN TRANSACTION;");

    }
}

void Sql::connClose(){

#if defined (Sql_Debug_Commit_Time)
    QElapsedTimer tim;
    tim.start();
#endif
        db.commit();
#if defined (Sql_Debug_Commit_Time)
    qDebug()<<tim.elapsed()<< " ms for COMMIT";
#endif
    //sqlcommand("END TRANSACTION;");
    QString connection;
    connection = db.connectionName();
    db.close();
    #if defined (Sql_Debug_DB_Status)
           qDebug()<<" ";
           qDebug()<<"DB connection closed";
    #endif
    query->clear();
}

void Sql::sqlcommand (QString command){
    bool needsToClose=false;
    /*if(!db.isOpen()){
        needsToClose=true;
        connOpen();
    }*/
    //connClose();
    //connOpen();*/
    try {
    #if defined(Sql_Debug_Time)
        QElapsedTimer tim;
        tim.start();
    #endif


    if(!query->exec(command)){
        throw("command failed to execut : "+command);
    }

    #if defined(Sql_Debug_Time)
        if(Sql_Debug_MinTime<tim.elapsed()){
        qDebug()<<tim.elapsed()<< " ms for:";
        qDebug()<<command;}
    #endif
    query->first();
    }
    catch  (QString error) {
        qDebug()<<"SQL ERROR sqlcommand()::"+error;
        //throw "sqlcommand()::"+error;
        }
    /*if(needsToClose){
        connClose();
    }*/
}
void Sql::insertIntoTable (QString tableName,vector <QString> columns,vector <QString> type,
    vector <QString> valuesName){
    vector<vector <QString>> vec;
    vec.push_back(valuesName);
    insertIntoTable(tableName,columns,type,vec);
}

void Sql::insertIntoTable (QString tableName,vector <QString> columns,vector <QString> type,
    vector<vector <QString>> valuesName){
    /*if(!dontClose)
        connOpen();*/
    if(columns.size()!=valuesName[0].size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  values size "<<valuesName[0].size()<<" don't match";
    }
    for (unsigned i=0;i<type.size();i++) {
        if(type[i]==Sql_Type_TEXT){
            for (int j=0;j<valuesName.size();j++) {
                valuesName[j][i]=" \""+valuesName[j][i]+" \"";
            }
        }
    }
    QString command;
    command ="INSERT INTO "+tableName;
    command +=getBrackedString(columns);
    command += " VALUES ";
    command +=getBrackedString(valuesName);
    command += ";";
    sqlcommand(command);
    /*/*if(!dontClose)
        connClose();*/
}

void Sql::prepeareInsertTable(QString tableName,vector<QString> columns){
    QString command;
    command ="INSERT INTO "+tableName;
    command +=getBrackedString(columns);
    command += " VALUES ";

    vector <QString> question;

    for(unsigned i=0;i<columns.size();i++){
        question.push_back(":"+columns[i]);
    }
    command +=getBrackedString(question);
    query->prepare(command);
}
/*
void Sql::insertIntoTable (QString tableName,vector <QString> columns,vector <QString> type,
    vector <QVariant> value){
    vector<vector <QVariant>> vec;
    vec.push_back(value);
    insertIntoTable(tableName,columns,type,vec);
}

void Sql::insertIntoTable (QString tableName,vector <QString> columns,vector <QString> type,
    vector<vector <QVariant>> value){
    QSqlQuery query->(db);
    /*if(!dontClose)
        connOpen();
    if(columns.size()!=value[0].size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  values size "<<value[0].size()<<" don't match";
    }
    QString command;
    command ="INSERT INTO "+tableName;
    command +=getBrackedString(columns);
    command += " VALUES ";

    vector <QString> question;

    for(unsigned i=0;i<columns.size();i++){
        question.push_back(":"+columns[i]);
    }command += " VALUES ";
    command +=getBrackedString(question);
    command += ";";
    query->->prepare(command);
    qDebug()<<command;
    for(unsigned j=0;j<value.size();j++){
        for(unsigned i=0;i<columns.size();i++){
            if(type[i]==Sql_Type_INT){
                query->->bindValue(question[i],value[j][i].toInt());}
            else if(type[i]==Sql_Type_TEXT){
                query->->bindValue(question[i],value[j][i].toString());}
            else if(type[i]==Sql_Type_BOOL){
                query->->bindValue(question[i],value[j][i].toBool());}
            else if(type[i]==Sql_Type_REAL){
                query->->bindValue(question[i],value[j][i].toReal());}

        }
        query->->exec();
    }
    //sqlcommand(command);
    if(!dontClose)
        connClose();
}*/

bool Sql::createTable (QString tableName,vector <QString> columns,vector <QString> type){
    if(columns.size()!=type.size()){
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  type size "<<type.size()<<" don't match";
        return false;
    }
    QString command;
    command +="CREATE TABLE IF NOT EXISTS "+tableName+" (";
    //command +=ID_Column;
   // command +=" INT IDENTITY(1,1) PRIMARY KEY,";
    for(unsigned long long i=0;i<type.size();i++){
        if(type[i]==Sql_Type_BOOL||type[i]==Sql_Type_TEXT
        ||type[i]==Sql_Type_INT||type[i]==Sql_Type_REAL
        ||type[i]==Sql_Type_DATE){
            command +=columns[i]+" "+type[i];
            if(i<type.size()-1){
                command += " , ";
            }
        }
        else{
           qDebug()<<" sql Type "+type[i]+" unknown";
           return false;
        }
    }
    command+=");";
    sqlcommand(command);
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

unsigned Sql::getFreeId(QString table,QString idColumn){
    QString command;
    command="SELECT "+idColumn+" FROM "+table+" ORDER BY "+idColumn+" ASC";
    qDebug()<<command;
    sqlcommand(command);
    unsigned id=0;
    for(int i=0;i<countEnteries(query);i++){
        query->next();
        id+=1;
        if(query->value(idColumn).toInt()!=id){
            return id;
        }
    }
    return id+1;
}

QString Sql::getBrackedString(vector <QString> data){
    QString res;
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

QString Sql::getBrackedString(vector<vector <QString>> data){
    QString res;
    for(unsigned long long i=0;i<data.size();i++){
        res +=this->getBrackedString(data[i]);
        if(i<data.size()-1){
            res += ", ";
        }
    }
    return res;
}

void Sql::setHostName(QString argHost){
    host=argHost;
}
void Sql::setPort(unsigned argPort){
    port=argPort;
}
void Sql::setDatabaseName(QString argDatabaseName){
    databaseName=argDatabaseName;
}
void Sql::setUserName(QString argUserName){
    userName=argUserName;
}
void Sql::setPassword(QString argPassword){
    password=argPassword;
}

QString Sql::getHostName(){
    return host;
}
unsigned Sql::getPort(){
    return port;
}
QString Sql::getDatabaseName(){
    return databaseName;
}
QString Sql::getUserName(){
    return userName;
}
QString Sql::getPassword(){
    return password;
}
