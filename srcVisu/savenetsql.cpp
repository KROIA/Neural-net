#include "savenetsql.h"

SaveNetSql::SaveNetSql(QString _path )
{
    netTable={Inputs_Column,HiddenXs_Column,HiddenYs_Column,Outputs_Column};
    dbPath="C:/Users/Hannes/Desktop/savedb.db";
    createDb();
    //connOpen();
    //connClose();
}


void SaveNetSql::connOpen(){

    mydb = QSqlDatabase::addDatabase("QSQLITE","connction1");
    //mydb = QSqlDatabase::database("connction1");
    mydb.setDatabaseName(dbPath);

    if (!mydb.open())
    {
    mydb.setDatabaseName(dbPath);
      if (!mydb.open()){

          qDebug()<<"DB is not connected.";

}
    }
    else
    {
      //qDebug()<<"DB is connected.";
    }
}


void SaveNetSql::connClose(){
    QString connection;
    connection = mydb.connectionName();
    mydb.close();
    mydb = QSqlDatabase();
    mydb.removeDatabase(connection);
}



void SaveNetSql::saveNet(Net* saveNet){
    QString tableName=Net_Table_Name;
    vector<QString> values;
    values.push_back(QString::number(saveNet->get_inputNeurons()));
    values.push_back(QString::number(saveNet->get_hiddenNeuronsX()));
    values.push_back(QString::number(saveNet->get_hiddenNeuronsY()));
    values.push_back(QString::number(saveNet->get_outputNeurons()));
    isertIntoTable(tableName,netTable,values);
}
void SaveNetSql::saveNetVec(vector<Net*> saveNet){
    for(unsigned long long i=0;i<saveNet.size();i++){
        //saveNet(saveNet[i]);
    }
}
void SaveNetSql::createDb(){
vector<QString> types={Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,Sql_Type_INT};
    createTable(Net_Table_Name,netTable,types);
}


void SaveNetSql::sqlcommandi(QString command){
    connOpen();
    QSqlQuery _query(mydb);
    qDebug()<<command;
    qDebug()<<"";
    _query.exec(command);
    query=_query;
    _query.clear();
    query.first();
}

void SaveNetSql::sqlcommand(QString command){
    sqlcommandi(command);
    connClose();
}

bool SaveNetSql::createTable(QString tableName,vector<QString> columns,vector<QString> type){
    if(columns.size()!=type.size()){
        qDebug()<<" column size "<<columns.size()<<" and  type size "<<type.size()<<" don't match";
        return false;
    }
    QString command;
    command +="CREATE TABLE IF NOT EXISTS "+tableName+" (";
    for(unsigned long long i=0;i<type.size();i++){
        if(type[i]==Sql_Type_BOOL||type[i]==Sql_Type_TEXT||type[i]==Sql_Type_INT){
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
    command += ");";
    sqlcommand(command);
    return true;
}
bool SaveNetSql::isertIntoTable(QString tableName,vector<QString> columns,vector<QString> valuesName){
    if(columns.size()!=valuesName.size()){
        qDebug()<<" column size "<<columns.size()<<" and  values size "<<valuesName.size()<<" don't match";
        return false;
    }
    QString command;
    command +="INSERT INTO "+tableName+" (";
    for(unsigned long long i=0;i<columns.size();i++){
        command +=columns[i];
        if(i<columns.size()-1){
            command += " , ";
        }
    }
    command += ") VALUES(";
    for(unsigned long long i=0;i<valuesName.size();i++){
        command +=valuesName[i];
        if(i<valuesName.size()-1){
            command += " , ";
        }
    }

    command += ");";
    //qDebug()<<command;
    sqlcommand(command);
    return true;
}
