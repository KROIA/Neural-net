#include "savenetsql.h"

SaveNetSql::SaveNetSql(QString _path )
{
    netTable={  Net_Table_ID_Column,Inputs_Column,HiddenXs_Column,
                HiddenYs_Column,Outputs_Column,
                Bias_Column,Bias_Value_Column,
                Activation_Function_Column};
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
    int next=findFreeId(Net_Table_Name,Net_Table_ID_Column);
    qDebug()<<" Free "<<next;
    saveNetId(saveNet,next);
}

void SaveNetSql::saveNetId(Net* saveNet,int id){
    QString tableName=Net_Table_Name;
    vector<QString> values;
    values.push_back(QString::number(id));
    values.push_back(QString::number(saveNet->get_inputNeurons()));
    values.push_back(QString::number(saveNet->get_hiddenNeuronsX()));
    values.push_back(QString::number(saveNet->get_hiddenNeuronsY()));
    values.push_back(QString::number(saveNet->get_outputNeurons()));
    values.push_back(QString::number(saveNet->get_bias()));
    values.push_back(QString::number(saveNet->get_biasValue()));
    values.push_back(QString::number(saveNet->get_activationFunction()));
    isertIntoTable(tableName,netTable,values);
    connClose();
}
void SaveNetSql::saveNetVec(vector<Net*> saveNet){
    for(unsigned long long i=0;i<saveNet.size();i++){
        //saveNet(saveNet[i]);
    }
}
void SaveNetSql::createDb(){
    vector<QString> types={ Sql_Type_INT,Sql_Type_INT,Sql_Type_INT,
                            Sql_Type_INT,Sql_Type_INT,
                            Sql_Type_BOOL,Sql_Type_REAL,
                            Sql_Type_INT};
    createTable(Net_Table_Name,netTable,types);

    types.clear();
    types={

    };

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
        qDebug()<<"ERROR SQL: column size "<<columns.size()<<" and  type size "<<type.size()<<" don't match";
        qDebug()<<type;
        return false;
    }
    QString command;
    command +="CREATE TABLE IF NOT EXISTS "+tableName+" (";
    for(unsigned long long i=0;i<type.size();i++){
        if(type[i]==Sql_Type_BOOL||type[i]==Sql_Type_TEXT
        ||type[i]==Sql_Type_INT||type[i]==Sql_Type_REAL){
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
int SaveNetSql::findFreeId(QString tableName,QString columnId){
    QString command;
    command += "Select "+columnId+" FROM "+tableName;
    command += " ORDER BY "+columnId+" ASC";
    sqlcommandi(command);
    int value=0;
    query.first();
    do{
        qDebug()<<query.value(columnId).toInt()<<" "<<value;
        if(query.value(columnId).toInt()==value){
            value=query.value(columnId).toInt()+1;
        }
    }while (query.next());
    connClose();
    return value;
}
